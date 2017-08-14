#include "RTSPluginPrivatePCH.h"
#include "RTSGathererComponent.h"

#include "GameFramework/Actor.h"

#include "RTSResourceSourceComponent.h"
#include "RTSResourceDrainComponent.h"


URTSGathererComponent::URTSGathererComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URTSGathererComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
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

bool URTSGathererComponent::CanGatherFrom(AActor* ResourceSource)
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
	float ClosestResourceDrainDistance;

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

AActor* URTSGathererComponent::GetPreviousResourceSource() const
{
	return PreviousResourceSource;
}

float URTSGathererComponent::GetGatherRange(AActor* ResourceSource)
{
	FRTSGatherData GatherData;
	if (!GetGatherDataForResourceSource(ResourceSource, &GatherData))
	{
		return 0.0f;
	}

	return GatherData.Range;
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
	float GatheredResources = ResourceSourceComponent->ExtractResources(GetOwner(), AmountToGather);

	CarriedResourceAmount += GatheredResources;

	// Start cooldown timer.
	RemainingCooldown = GatherData.Cooldown;

	UE_LOG(RTSLog, Log, TEXT("Actor %s gathered %f %s from %s."),
		*GetOwner()->GetName(),
		GatheredResources,
		*GatherData.ResourceType->GetName(),
		*ResourceSource->GetName());

	// Notify listeners.
	OnResourcesGathered.Broadcast(ResourceSource, GatherData, GatheredResources);

	if (GatherData.bNeedsReturnToDrain)
	{
		// Check if we're at capacity.
		if (CarriedResourceAmount >= GatherData.Capacity)
		{
			// Stop gathering.
			PreviousResourceSource = CurrentResourceSource;
			CurrentResourceSource = nullptr;
		}
	}
	else
	{
		// Check if we're at capacity or resource source depleted.
		if (CarriedResourceAmount >= GatherData.Capacity || !IsValid(ResourceSource))
		{
			// Return immediately.
			auto OwningPlayer = Cast<ARTSPlayerController>(GetOwner()->GetOwner());

			if (OwningPlayer)
			{
				float ReturnedResources = OwningPlayer->AddResources(CarriedResourceType, CarriedResourceAmount);

				if (ReturnedResources > 0.0f)
				{
					CarriedResourceAmount -= ReturnedResources;

					UE_LOG(RTSLog, Log, TEXT("Actor %s returned %f %s without returning to drain."),
						*GetOwner()->GetName(),
						ReturnedResources,
						*CarriedResourceType->GetName());

					// Notify listeners.
					OnResourcesReturned.Broadcast(nullptr, CarriedResourceType, ReturnedResources);
				}
			}
		}

		if (!IsValid(ResourceSource))
		{
			// Stop gathering.
			PreviousResourceSource = CurrentResourceSource;
			CurrentResourceSource = nullptr;
		}
	}

	return GatheredResources;
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

	UE_LOG(RTSLog, Log, TEXT("Actor %s returned %f %s to %s."),
		*GetOwner()->GetName(),
		ReturnedResources,
		*CarriedResourceType->GetName(),
		*ResourceDrain->GetName());

	// Notify listeners.
	OnResourcesReturned.Broadcast(ResourceDrain, CarriedResourceType, ReturnedResources);
	return ReturnedResources;
}

bool URTSGathererComponent::GetGatherDataForResourceSource(AActor* ResourceSource, FRTSGatherData* OutGatherData)
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

bool URTSGathererComponent::GetGatherDataForResourceType(TSubclassOf<URTSResourceType> ResourceType, FRTSGatherData* OutGatherData)
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
