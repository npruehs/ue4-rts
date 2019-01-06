#include "RealTimeStrategyPCH.h"
#include "RTSGathererComponent.h"

#include "GameFramework/Actor.h"

#include "RTSContainerComponent.h"
#include "RTSPlayerResourcesComponent.h"
#include "RTSResourceSourceComponent.h"
#include "RTSResourceDrainComponent.h"
#include "RTSUtilities.h"


URTSGathererComponent::URTSGathererComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicated(true);

	// Set reasonable default values.
	ResourceSweepRadius = 1000.0f;
}

void URTSGathererComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URTSGathererComponent, CarriedResourceAmount);
	DOREPLIFETIME(URTSGathererComponent, CarriedResourceType);
}

void URTSGathererComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	if (!CurrentResourceSource)
	{
		return;
	}

	// Check range.
	float GatherRange = GetGatherRange(CurrentResourceSource);

	if (URTSUtilities::GetActorDistance(GetOwner(), CurrentResourceSource, true) > GatherRange)
	{
		// Stop gathering.
		CurrentResourceSource = nullptr;
		return;
	}

	// Update cooldown timer.
	if (RemainingCooldown > 0)
	{
		RemainingCooldown -= DeltaTime;

		if (RemainingCooldown <= 0)
		{
			GatherResources(CurrentResourceSource);
		}
	}
}

bool URTSGathererComponent::CanGatherFrom(AActor* ResourceSource) const
{
	if (!IsValid(ResourceSource))
	{
		return false;
	}

	// Check source type.
	if (!ResourceSourceActorClasses.Contains(ResourceSource->GetClass()))
	{
		return false;
	}

	// Check capacity.
	FRTSGatherData GatherData;
	if (!GetGatherDataForResourceSource(ResourceSource, &GatherData))
	{
		return false;
	}

	return CarriedResourceAmount < GatherData.Capacity;
}

AActor* URTSGathererComponent::FindClosestResourceDrain() const
{
	// Find nearby actors.
	AActor* ClosestResourceDrain = nullptr;
	float ClosestResourceDrainDistance = 0.0f;

	for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		auto Gatherer = GetOwner();
		auto ResourceDrain = *ActorItr;

		// Check if found resource drain.
		auto ResourceDrainComponent = ResourceDrain->FindComponentByClass<URTSResourceDrainComponent>();

		if (!ResourceDrainComponent)
		{
			continue;
		}

		// Check owner.
		auto GathererOwnerComponent = Gatherer->FindComponentByClass<URTSOwnerComponent>();

		if (!GathererOwnerComponent || !GathererOwnerComponent->IsSameTeamAsActor(ResourceDrain))
		{
			continue;
		}

		// Check resource type.
		if (!ResourceDrainComponent->ResourceTypes.Contains(CarriedResourceType))
		{
			continue;
		}
	
		// Check if ready to use (e.g. construction finished).
		if (!URTSUtilities::IsReadyToUse(ResourceDrain))
		{
			continue;
		}

		// Check distance.
		float Distance = Gatherer->GetDistanceTo(ResourceDrain);

		if (!ClosestResourceDrain || Distance < ClosestResourceDrainDistance)
		{
			ClosestResourceDrain = ResourceDrain;
			ClosestResourceDrainDistance = Distance;
		}
	}

	return ClosestResourceDrain;
}

AActor* URTSGathererComponent::GetPreferredResourceSource() const
{
    if (IsValid(PreviousResourceSource))
    {
        return PreviousResourceSource;
    }

    return GetClosestResourceSource(PreviousResourceType, ResourceSweepRadius);
}

AActor* URTSGathererComponent::GetClosestResourceSource(TSubclassOf<class URTSResourceType> DesiredResourceType, float MaxDistance) const
{
    // Sweep for sources.
    AActor* ClosestResourceSource = nullptr;
    float ClosestResourceSourceDistance = 0.0f;

    for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
    {
        auto Gatherer = GetOwner();
        auto ResourceSource = *ActorItr;

        // Check if found resource source.
        auto ResourceSourceComponent = ResourceSource->FindComponentByClass<URTSResourceSourceComponent>();

        if (!ResourceSourceComponent)
        {
            continue;
        }

        // Check resource type.
        if (ResourceSourceComponent->ResourceType != DesiredResourceType)
        {
            continue;
        }

        // Check distance.
        float Distance = Gatherer->GetDistanceTo(ResourceSource);

        if (MaxDistance > 0.0f && Distance > MaxDistance)
        {
            continue;
        }
        
        if (!ClosestResourceSource || Distance < ClosestResourceSourceDistance)
        {
            ClosestResourceSource = ResourceSource;
            ClosestResourceSourceDistance = Distance;
        }
    }

    return ClosestResourceSource;
}

float URTSGathererComponent::GetGatherRange(AActor* ResourceSource) const
{
	FRTSGatherData GatherData;
	if (!GetGatherDataForResourceSource(ResourceSource, &GatherData))
	{
		return 0.0f;
	}

	return GatherData.Range;
}

bool URTSGathererComponent::IsCarryingResources() const
{
	return CarriedResourceAmount > 0;
}

bool URTSGathererComponent::IsGathering() const
{
	return CurrentResourceSource != nullptr;
}

void URTSGathererComponent::StartGatheringResources(AActor* ResourceSource)
{
	if (!CanGatherFrom(ResourceSource))
	{
		return;
	}

	if (CurrentResourceSource == ResourceSource)
	{
		return;
	}

	CurrentResourceSource = ResourceSource;

	// Get resource type.
	auto ResourceSourceComponent = ResourceSource->FindComponentByClass<URTSResourceSourceComponent>();

	if (!ResourceSourceComponent)
	{
		return;
	}

	FRTSGatherData GatherData;
	if (!GetGatherDataForResourceSource(ResourceSource, &GatherData))
	{
		return;
	}

	// Reset carried amount.
	CarriedResourceAmount = 0.0f;
	CarriedResourceType = ResourceSourceComponent->ResourceType;

	// Start cooldown before first gathering.
	RemainingCooldown = GatherData.Cooldown;

	if (ResourceSourceComponent->bGathererMustEnter)
	{
		// Enter resource source.
		auto ContainerComponent = ResourceSource->FindComponentByClass<URTSContainerComponent>();

		if (ContainerComponent)
		{
			ContainerComponent->LoadActor(GetOwner());
		}
	}
}

float URTSGathererComponent::GatherResources(AActor* ResourceSource)
{
	if (!IsValid(ResourceSource))
	{
		return 0.0f;
	}

	// Check cooldown.
	if (RemainingCooldown > 0)
	{
		return 0.0f;
	}

	// Check resource type.
	FRTSGatherData GatherData;
	if (!GetGatherDataForResourceSource(ResourceSource, &GatherData))
	{
		return 0.0f;
	}

	// Determine amount to gather.
	float AmountToGather = GatherData.AmountPerGathering;

	if (CarriedResourceAmount + AmountToGather >= GatherData.Capacity)
	{
		// Capacity limit hit. Clamp gathered resources.
		AmountToGather = GatherData.Capacity - CarriedResourceAmount;
	}

	// Gather resources.
	auto ResourceSourceComponent = ResourceSource->FindComponentByClass<URTSResourceSourceComponent>();
	float GatheredResourceAmount = ResourceSourceComponent->ExtractResources(GetOwner(), AmountToGather);

	CarriedResourceAmount += GatheredResourceAmount;

	// Start cooldown timer.
	RemainingCooldown = GatherData.Cooldown;

	UE_LOG(LogRTS, Log, TEXT("Actor %s gathered %f %s from %s."),
		*GetOwner()->GetName(),
        GatheredResourceAmount,
		*GatherData.ResourceType->GetName(),
		*ResourceSource->GetName());

	// Notify listeners.
	OnResourcesGathered.Broadcast(ResourceSource, GatherData, GatheredResourceAmount);

	if (GatherData.bNeedsReturnToDrain)
	{
		// Check if we're at capacity.
		if (CarriedResourceAmount >= GatherData.Capacity)
		{
			// Stop gathering.
			LeaveCurrentResourceSource();
		}
	}
	else
	{
		// Check if we're at capacity or resource source depleted.
		if (CarriedResourceAmount >= GatherData.Capacity || !IsValid(ResourceSource))
		{
			// Return immediately.
			auto Owner = GetOwner()->GetOwner();

			if (Owner)
			{
                auto PlayerResourcesComponent = Owner->FindComponentByClass<URTSPlayerResourcesComponent>();

                if (PlayerResourcesComponent)
                {
                    float ReturnedResources = PlayerResourcesComponent->AddResources(CarriedResourceType, CarriedResourceAmount);

                    if (ReturnedResources > 0.0f)
                    {
                        CarriedResourceAmount -= ReturnedResources;

                        UE_LOG(LogRTS, Log, TEXT("Actor %s returned %f %s without returning to drain."),
                            *GetOwner()->GetName(),
                            ReturnedResources,
                            *CarriedResourceType->GetName());

                        // Notify listeners.
                        OnResourcesReturned.Broadcast(nullptr, CarriedResourceType, ReturnedResources);
                    }
                }
			}
		}

		if (!IsValid(ResourceSource))
		{
			// Stop gathering.
			LeaveCurrentResourceSource();
		}
	}

	return GatheredResourceAmount;
}

float URTSGathererComponent::ReturnResources(AActor* ResourceDrain)
{
	if (!IsValid(ResourceDrain))
	{
		return 0.0f;
	}

	// Return resources.
	auto ResourceDrainComponent = ResourceDrain->FindComponentByClass<URTSResourceDrainComponent>();
	float ReturnedResources = ResourceDrainComponent->ReturnResources(GetOwner(), CarriedResourceType, CarriedResourceAmount);

	CarriedResourceAmount -= ReturnedResources;

	UE_LOG(LogRTS, Log, TEXT("Actor %s returned %f %s to %s."),
		*GetOwner()->GetName(),
		ReturnedResources,
		*CarriedResourceType->GetName(),
		*ResourceDrain->GetName());

	// Notify listeners.
	OnResourcesReturned.Broadcast(ResourceDrain, CarriedResourceType, ReturnedResources);
	return ReturnedResources;
}

bool URTSGathererComponent::GetGatherDataForResourceSource(AActor* ResourceSource, FRTSGatherData* OutGatherData) const
{
	if (!IsValid(ResourceSource))
	{
		return false;
	}

	auto ResourceSourceComponent = ResourceSource->FindComponentByClass<URTSResourceSourceComponent>();

	if (!ResourceSourceComponent)
	{
		return false;
	}

	return GetGatherDataForResourceType(ResourceSourceComponent->ResourceType, OutGatherData);
}

bool URTSGathererComponent::GetGatherDataForResourceType(TSubclassOf<URTSResourceType> ResourceType, FRTSGatherData* OutGatherData) const
{
	for (const FRTSGatherData& GatherData : GatheredResources)
	{
		if (GatherData.ResourceType == ResourceType)
		{
			OutGatherData->AmountPerGathering = GatherData.AmountPerGathering;
			OutGatherData->bNeedsReturnToDrain = GatherData.bNeedsReturnToDrain;
			OutGatherData->Capacity = GatherData.Capacity;
			OutGatherData->Cooldown = GatherData.Cooldown;
			OutGatherData->Range = GatherData.Range;
			OutGatherData->ResourceType = GatherData.ResourceType;
			return true;
		}
	}

	return false;
}

void URTSGathererComponent::LeaveCurrentResourceSource()
{
	if (!CurrentResourceSource)
	{
		return;
	}

	// Leave resource source.
	auto ContainerComponent = CurrentResourceSource->FindComponentByClass<URTSContainerComponent>();

	if (ContainerComponent)
	{
		ContainerComponent->UnloadActor(GetOwner());
	}

	// Store data about resource source for future reference (e.g. return here, or find similar).
	PreviousResourceSource = CurrentResourceSource;
	CurrentResourceSource = nullptr;

	PreviousResourceType = CarriedResourceType;
}
