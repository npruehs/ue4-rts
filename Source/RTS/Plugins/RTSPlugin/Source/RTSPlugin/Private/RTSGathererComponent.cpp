#include "RTSPluginPrivatePCH.h"
#include "RTSGathererComponent.h"

#include "GameFramework/Actor.h"

#include "RTSResourceSourceComponent.h"


URTSGathererComponent::URTSGathererComponent(const FObjectInitializer& ObjectInitializer)
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

void URTSGathererComponent::GatherResources(AActor* ResourceSource)
{
	if (!IsValid(ResourceSource))
	{
		return;
	}

	// Check cooldown.
	if (RemainingCooldown > 0)
	{
		return;
	}

	// Check resource type.
	FRTSGatherData GatherData;
	if (!GetGatherDataForResourceSource(ResourceSource, &GatherData))
	{
		return;
	}

	// Determine amount to gather.
	float AmountToGather = GatherData.AmountPerGathering;

	if (CarriedResourceAmount + AmountToGather > GatherData.Capacity)
	{
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
	OnResourcesGathered.Broadcast(GatherData, ResourceSource, GatheredResources);
}

bool URTSGathererComponent::GetGatherDataForResourceSource(AActor* ResourceSource, FRTSGatherData* OutGatherData)
{
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
