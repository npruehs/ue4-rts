#include "RTSPluginPrivatePCH.h"
#include "RTSConstructionSiteComponent.h"

#include "GameFramework/Actor.h"

#include "RTSBuilderComponent.h"


URTSConstructionSiteComponent::URTSConstructionSiteComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;

	State = ERTSConstructionState::CONSTRUCTIONSTATE_NotStarted;
}

void URTSConstructionSiteComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URTSConstructionSiteComponent, RemainingConstructionTime);
}

void URTSConstructionSiteComponent::BeginPlay()
{
	UActorComponent::BeginPlay();

	if (bStartImmediately)
	{
		StartConstruction();
	}
}

void URTSConstructionSiteComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	UActorComponent::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (State == ERTSConstructionState::CONSTRUCTIONSTATE_Finished)
	{
		return;
	}

	// Compute construction progress based on number of assigned builders.
	float ConstructionProgress = (DeltaTime * ProgressMadeAutomatically) + (DeltaTime * ProgressMadePerBuilder * AssignedBuilders.Num());

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

	RemainingConstructionTime = ConstructionTime;
	State = ERTSConstructionState::CONSTRUCTIONSTATE_Constructing;

	UE_LOG(RTSLog, Log, TEXT("Construction %s started."), *GetName());

	// Notify listeners.
	OnConstructionStarted.Broadcast(ConstructionTime);
}

void URTSConstructionSiteComponent::FinishConstruction()
{
	RemainingConstructionTime = 0;
	State = ERTSConstructionState::CONSTRUCTIONSTATE_Finished;

	UE_LOG(RTSLog, Log, TEXT("Construction %s finished."), *GetName());

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
	UE_LOG(RTSLog, Log, TEXT("Construction %s cancelled."), *GetName());

	// Destroy construction site.
	GetOwner()->Destroy();

	// Notify listeners.
	OnConstructionCanceled.Broadcast();
}
