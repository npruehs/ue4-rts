#include "RTSPluginPCH.h"
#include "RTSConstructionSiteComponent.h"

#include "GameFramework/Actor.h"

#include "RTSBuilderComponent.h"
#include "RTSContainerComponent.h"
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

	// Check for autostart.
	if (State == ERTSConstructionState::CONSTRUCTIONSTATE_NotStarted && bStartImmediately)
	{
		StartConstruction();
	}
	else if (State == ERTSConstructionState::CONSTRUCTIONSTATE_Finished)
	{
		return;
	}

	// Compute construction progress based on number of assigned builders.
	float ConstructionProgress = (DeltaTime * ProgressMadeAutomatically) + (DeltaTime * ProgressMadePerBuilder * AssignedBuilders.Num());

	// Check construction costs.
	bool bConstructionCostPaid = false;

	if (ConstructionCostType == ERTSProductionCostType::COST_PayOverTime)
	{
		auto PlayerController = Cast<ARTSPlayerController>(GetOwner()->GetOwner());

		if (!PlayerController)
		{
			UE_LOG(LogRTS, Error, TEXT("%s needs to pay for construction, but has no owning player."), *GetOwner()->GetName());
			return;
		}

		bool bCanPayAllConstructionCosts = true;

		for (auto& Resource : ConstructionCosts)
		{
			float ResourceAmount = Resource.Value * ConstructionProgress / ConstructionTime;

			if (!PlayerController->CanPayResources(Resource.Key, ResourceAmount))
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
				PlayerController->PayResources(Resource.Key, ResourceAmount);
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
		auto PlayerController = Cast<ARTSPlayerController>(GetOwner()->GetOwner());

		if (!PlayerController)
		{
			UE_LOG(LogRTS, Error, TEXT("%s needs to pay for construction, but has no owning player."), *GetOwner()->GetName());
			CancelConstruction();
			return;
		}

		if (!PlayerController->CanPayAllResources(ConstructionCosts))
		{
			UE_LOG(LogRTS, Error, TEXT("%s needs to pay for constructing %s, but does not have enough resources."),
				*GetOwner()->GetName(),
				*GetName());
			CancelConstruction();
			return;
		}

		// Pay construction costs.
		PlayerController->PayAllResources(ConstructionCosts);
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

	UE_LOG(LogRTS, Log, TEXT("Construction %s finished."), *GetName());

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
	auto PlayerController = Cast<ARTSPlayerController>(GetOwner()->GetOwner());

	if (PlayerController)
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

			PlayerController->AddResources(ResourceType, ResourceAmount);

			UE_LOG(LogRTS, Log, TEXT("%f %s of construction costs refunded."), ResourceAmount, *ResourceType->GetName());

			// Notify listeners.
			OnConstructionCostRefunded.Broadcast(ResourceType, ResourceAmount);
		}
	}

	// Destroy construction site.
	GetOwner()->Destroy();

	// Notify listeners.
	OnConstructionCanceled.Broadcast();
}
