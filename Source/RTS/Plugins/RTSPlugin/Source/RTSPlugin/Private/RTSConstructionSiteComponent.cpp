#include "RTSPluginPCH.h"
#include "RTSConstructionSiteComponent.h"

#include "GameFramework/Actor.h"

#include "RTSBuilderComponent.h"
#include "RTSContainerComponent.h"
#include "RTSPlayerAdvantageComponent.h"
#include "RTSPlayerResourcesComponent.h"
#include "RTSResourceType.h"


URTSConstructionSiteComponent::URTSConstructionSiteComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicated(true);

	State = ERTSConstructionState::CONSTRUCTIONSTATE_NotStarted;
	RefundFactor = 0.5f;
}

void URTSConstructionSiteComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URTSConstructionSiteComponent, RemainingConstructionTime);
    DOREPLIFETIME(URTSConstructionSiteComponent, State);
}

void URTSConstructionSiteComponent::BeginPlay()
{
	UActorComponent::BeginPlay();

	// Set container size.
	auto ContainerComponent = GetOwner()->FindComponentByClass<URTSContainerComponent>();

	if (ContainerComponent)
	{
		ContainerComponent->Capacity = MaxAssignedBuilders;
	}
}

void URTSConstructionSiteComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	UActorComponent::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Don't update construction progress on clients - will be replicated from server.
    if (GetNetMode() == NM_Client)
    {
        return;
    }

	// Check for autostart.
	if (State == ERTSConstructionState::CONSTRUCTIONSTATE_NotStarted && bStartImmediately)
	{
		StartConstruction();
	}
	else if (State == ERTSConstructionState::CONSTRUCTIONSTATE_Finished)
	{
		return;
	}

    // Check for speed boosts.
    float SpeedBoostFactor = 1.0f;
    AActor* OwningActor = GetOwner();

    if (OwningActor)
    {
        AActor* OwningPlayer = OwningActor->GetOwner();

        if (OwningPlayer)
        {
            URTSPlayerAdvantageComponent* PlayerAdvantageComponent = OwningPlayer->FindComponentByClass<URTSPlayerAdvantageComponent>();

            if (PlayerAdvantageComponent)
            {
                SpeedBoostFactor = PlayerAdvantageComponent->SpeedBoostFactor;
            }
        }
    }

	// Compute construction progress based on number of assigned builders.
	float ConstructionProgress =
        (DeltaTime * ProgressMadeAutomatically * SpeedBoostFactor) +
        (DeltaTime * ProgressMadePerBuilder * AssignedBuilders.Num() * SpeedBoostFactor);

	// Check construction costs.
	bool bConstructionCostPaid = false;

	if (ConstructionCostType == ERTSProductionCostType::COST_PayOverTime)
	{
        auto Owner = GetOwner()->GetOwner();

        if (!Owner)
        {
            UE_LOG(LogRTS, Error, TEXT("%s needs to pay for construction, but has no owner."), *Owner->GetName());
            return;
        }

        auto PlayerResourcesComponent = Owner->FindComponentByClass<URTSPlayerResourcesComponent>();

		if (!PlayerResourcesComponent)
		{
			UE_LOG(LogRTS, Error, TEXT("%s needs to pay for construction, but has no PlayerResourcesComponent."), *Owner->GetName());
			return;
		}

		bool bCanPayAllConstructionCosts = true;

		for (auto& Resource : ConstructionCosts)
		{
			float ResourceAmount = Resource.Value * ConstructionProgress / ConstructionTime;

			if (!PlayerResourcesComponent->CanPayResources(Resource.Key, ResourceAmount))
			{
				// Construction stopped until resources become available again.
				bCanPayAllConstructionCosts = false;
				break;
			}
		}

		if (bCanPayAllConstructionCosts)
		{
			// Pay construction costs.
			for (auto& Resource : ConstructionCosts)
			{
				float ResourceAmount = Resource.Value * ConstructionProgress / ConstructionTime;
                PlayerResourcesComponent->PayResources(Resource.Key, ResourceAmount);
			}

			bConstructionCostPaid = true;
		}
	}
	else
	{
		bConstructionCostPaid = true;
	}

	if (!bConstructionCostPaid)
	{
		return;
	}

	// Update construction progress.
	RemainingConstructionTime -= ConstructionProgress;

	// Check if finished.
	if (RemainingConstructionTime <= 0)
	{
		FinishConstruction();
	}
}

bool URTSConstructionSiteComponent::CanAssignBuilder(AActor* Builder) const
{
	return AssignedBuilders.Num() < MaxAssignedBuilders;
}

float URTSConstructionSiteComponent::GetProgressPercentage() const
{
	return 1 - (RemainingConstructionTime / ConstructionTime);
}

bool URTSConstructionSiteComponent::IsConstructing() const
{
	return State == ERTSConstructionState::CONSTRUCTIONSTATE_Constructing;
}

bool URTSConstructionSiteComponent::IsFinished() const
{
	return State == ERTSConstructionState::CONSTRUCTIONSTATE_Finished;
}

void URTSConstructionSiteComponent::StartConstruction()
{
	if (State != ERTSConstructionState::CONSTRUCTIONSTATE_NotStarted)
	{
		return;
	}

	// Check construction cost.
	if (ConstructionCostType == ERTSProductionCostType::COST_PayImmediately)
	{
        auto Owner = GetOwner()->GetOwner();

        if (!Owner)
        {
            UE_LOG(LogRTS, Error, TEXT("%s needs to pay for construction, but has no owner."), *Owner->GetName());
            return;
        }

        auto PlayerResourcesComponent = Owner->FindComponentByClass<URTSPlayerResourcesComponent>();

		if (!PlayerResourcesComponent)
		{
			UE_LOG(LogRTS, Error, TEXT("%s needs to pay for construction, but has no PlayerResourcesComponent."), *Owner->GetName());
			CancelConstruction();
			return;
		}

		if (!PlayerResourcesComponent->CanPayAllResources(ConstructionCosts))
		{
			UE_LOG(LogRTS, Error, TEXT("%s needs to pay for constructing %s, but does not have enough resources."),
				*Owner->GetName(),
				*GetOwner()->GetName());
			CancelConstruction();
			return;
		}

		// Pay construction costs.
        PlayerResourcesComponent->PayAllResources(ConstructionCosts);
	}

	// Start construction.
	RemainingConstructionTime = ConstructionTime;
	State = ERTSConstructionState::CONSTRUCTIONSTATE_Constructing;

	UE_LOG(LogRTS, Log, TEXT("Construction %s started."), *GetName());

	// Notify listeners.
	OnConstructionStarted.Broadcast(ConstructionTime);
}

void URTSConstructionSiteComponent::FinishConstruction()
{
	RemainingConstructionTime = 0;
	State = ERTSConstructionState::CONSTRUCTIONSTATE_Finished;

	UE_LOG(LogRTS, Log, TEXT("Construction %s finished."), *GetOwner()->GetName());

	// Notify builders.
	if (bConsumesBuilders)
	{
		for (AActor* Builder : AssignedBuilders)
		{
			Builder->Destroy();
			OnBuilderConsumed.Broadcast(Builder);
		}
	}

	// Notify listeners.
	OnConstructionFinished.Broadcast();
}

void URTSConstructionSiteComponent::CancelConstruction()
{
	if (IsFinished())
	{
		return;
	}

	UE_LOG(LogRTS, Log, TEXT("Construction %s canceled."), *GetName());

	// Refund resources.
    auto Owner = GetOwner()->GetOwner();

    if (Owner)
    {
        auto PlayerResourcesComponent = Owner->FindComponentByClass<URTSPlayerResourcesComponent>();

        if (!PlayerResourcesComponent)
        {
            float TimeRefundFactor = 0.0f;

            if (ConstructionCostType == ERTSProductionCostType::COST_PayImmediately)
            {
                TimeRefundFactor = 1.0f;
            }
            else if (ConstructionCostType == ERTSProductionCostType::COST_PayOverTime)
            {
                TimeRefundFactor = GetProgressPercentage();
            }

            float ActualRefundFactor = RefundFactor * TimeRefundFactor;

            // Refund construction costs.
            for (auto& Resource : ConstructionCosts)
            {
                TSubclassOf<URTSResourceType> ResourceType = Resource.Key;
                float ResourceAmount = Resource.Value * ActualRefundFactor;

                PlayerResourcesComponent->AddResources(ResourceType, ResourceAmount);

                UE_LOG(LogRTS, Log, TEXT("%f %s of construction costs refunded."), ResourceAmount, *ResourceType->GetName());

                // Notify listeners.
                OnConstructionCostRefunded.Broadcast(ResourceType, ResourceAmount);
            }
        }
    }

	// Destroy construction site.
	GetOwner()->Destroy();

	// Notify listeners.
	OnConstructionCanceled.Broadcast();
}
