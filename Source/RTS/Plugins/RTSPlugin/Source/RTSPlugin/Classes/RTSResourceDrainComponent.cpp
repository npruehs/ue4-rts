#include "RTSPluginPrivatePCH.h"
#include "RTSResourceDrainComponent.h"

#include "Net/UnrealNetwork.h"

#include "RTSPlayerController.h"


URTSResourceDrainComponent::URTSResourceDrainComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GathererCapacity = 1;
}

float URTSResourceDrainComponent::ReturnResources(AActor* Gatherer, TSubclassOf<URTSResourceType> ResourceType, float ResourceAmount)
{
	// Notify player.
	auto OwningPlayer = Cast<ARTSPlayerController>(GetOwner()->GetOwner());

	if (!OwningPlayer)
	{
		return 0.0f;
	}

	float ReturnedResources = OwningPlayer->AddResources(ResourceType, ResourceAmount);

	if (ReturnedResources <= 0.0f)
	{
		return 0.0f;
	}

	UE_LOG(RTSLog, Log, TEXT("Actor %s has returned %f resources of type %s to %s."),
		*Gatherer->GetName(),
		ReturnedResources,
		*ResourceType->GetName(),
		*GetOwner()->GetName());

	// Notify listeners.
	OnResourcesReturned.Broadcast(Gatherer, ResourceType, ReturnedResources);
	return ReturnedResources;
}
