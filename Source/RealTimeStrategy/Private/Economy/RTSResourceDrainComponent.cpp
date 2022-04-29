#include "Economy/RTSResourceDrainComponent.h"

#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"

#include "RTSLog.h"
#include "Economy/RTSPlayerResourcesComponent.h"
#include "Libraries/RTSGameplayTagLibrary.h"


URTSResourceDrainComponent::URTSResourceDrainComponent(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
    SetIsReplicatedByDefault(true);

	// Set reasonable default values.
	GathererCapacity = 1;
    InitialGameplayTags.AddTag(URTSGameplayTagLibrary::Status_Permanent_AcceptsReturnedResources());
}

float URTSResourceDrainComponent::ReturnResources(AActor* Gatherer, TSubclassOf<URTSResourceType> ResourceType, float ResourceAmount)
{
	// Notify player.
	auto Owner = GetOwner()->GetOwner();

	if (!Owner)
	{
		return 0.0f;
	}

    auto PlayerResourcesComponent = Owner->FindComponentByClass<URTSPlayerResourcesComponent>();

    if (!PlayerResourcesComponent)
    {
        return 0.0f;
    }

	float ReturnedResources = PlayerResourcesComponent->AddResources(ResourceType, ResourceAmount);

	if (ReturnedResources <= 0.0f)
	{
		return 0.0f;
	}

	// Notify listeners.
    NotifyOnResourcesReturned(Gatherer, ResourceType, ReturnedResources);
	return ReturnedResources;
}

TArray<TSubclassOf<URTSResourceType>> URTSResourceDrainComponent::GetResourceTypes() const
{
    return ResourceTypes;
}

bool URTSResourceDrainComponent::MustGathererEnter() const
{
    return bGathererMustEnter;
}

int32 URTSResourceDrainComponent::GetGathererCapacity() const
{
    return GathererCapacity;
}

void URTSResourceDrainComponent::NotifyOnResourcesReturned_Implementation(AActor* Gatherer, TSubclassOf<URTSResourceType> ResourceType, float ResourceAmount)
{
    UE_LOG(LogRTS, Log, TEXT("Actor %s has returned %f resources of type %s to %s."),
        *Gatherer->GetName(),
        ResourceAmount,
        *ResourceType->GetName(),
        *GetOwner()->GetName());

    OnResourcesReturned.Broadcast(GetOwner(), Gatherer, ResourceType, ResourceAmount);
}
