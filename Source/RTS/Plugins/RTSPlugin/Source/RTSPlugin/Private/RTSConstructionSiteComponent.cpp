#include "RTSPluginPrivatePCH.h"
#include "RTSConstructionSiteComponent.h"

#include "GameFramework/Actor.h"

#include "RTSBuilderComponent.h"


URTSConstructionSiteComponent::URTSConstructionSiteComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
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

	if (!bConstructing)
	{
		return;
	}

	if (RemainingConstructionTime <= 0)
	{
		return;
	}

	// Compute construction progress based on number of assigned builders.
	float ConstructionProgress = bRequiresBuilderAttention ? DeltaTime * AssignedBuilders.Num() : DeltaTime;

	// Update construction progress.
	RemainingConstructionTime -= ConstructionProgress;

	// Check if finished.
	if (RemainingConstructionTime <= 0)
	{
		RemainingConstructionTime = 0;
		bConstructing = false;

		UE_LOG(RTSLog, Log, TEXT("Construction %s finished."), *GetName());

		// Notify builders.
		if (bConsumesBuilders)
		{
			for (AActor* Builder : AssignedBuilders)
			{
				Builder->Destroy();
			}
		}

		for (AActor* Builder : AssignedBuilders)
		{
			if (!IsValid(Builder))
			{
				continue;
			}

			auto BuilderComponent = Builder->FindComponentByClass<URTSBuilderComponent>();

			if (!BuilderComponent)
			{
				continue;
			}

			BuilderComponent->LeaveConstructionSite();
		}

		// Notify listeners.
		OnConstructionFinished.Broadcast();
	}
}

float URTSConstructionSiteComponent::GetProgressPercentage() const
{
	return 1 - (RemainingConstructionTime / ConstructionTime);
}

bool URTSConstructionSiteComponent::IsConstructing() const
{
	return bConstructing;
}

void URTSConstructionSiteComponent::StartConstruction()
{
	if (bConstructing)
	{
		return;
	}

	RemainingConstructionTime = ConstructionTime;
	bConstructing = true;

	UE_LOG(RTSLog, Log, TEXT("Construction %s started."), *GetName());

	// Notify listeners.
	OnConstructionStarted.Broadcast(ConstructionTime);
}
