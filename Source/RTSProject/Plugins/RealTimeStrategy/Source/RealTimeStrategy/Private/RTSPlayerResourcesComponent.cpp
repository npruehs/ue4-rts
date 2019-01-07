#include "RealTimeStrategyPCH.h"
#include "RTSPlayerResourcesComponent.h"

#include "GameFramework/Actor.h"

#include "RTSResourceType.h"


URTSPlayerResourcesComponent::URTSPlayerResourcesComponent(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    SetIsReplicated(true);
}

void URTSPlayerResourcesComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(URTSPlayerResourcesComponent, ResourceAmounts);
    DOREPLIFETIME(URTSPlayerResourcesComponent, ResourceTypes);
}

void URTSPlayerResourcesComponent::BeginPlay()
{
    Super::BeginPlay();

    // Check resource types.
    int32 ResourceTypeNum = ResourceTypes.Num();
    int32 ResourceAmountNum = ResourceAmounts.Num();

    for (int32 Index = ResourceAmountNum; Index < ResourceTypeNum; ++Index)
    {
        UE_LOG(LogRTS, Warning, TEXT("Starting amount for resource type %s not set for player %s, assuming zero."),
            *ResourceTypes[Index]->GetName(),
            *GetOwner()->GetName());

        ResourceAmounts.Add(0);
    }
}

float URTSPlayerResourcesComponent::GetResources(TSubclassOf<URTSResourceType> ResourceType) const
{
    // Get current resource amount.
    int32 ResourceIndex = ResourceTypes.IndexOfByKey(ResourceType);

    if (ResourceIndex == INDEX_NONE)
    {
        UE_LOG(LogRTS, Error, TEXT("Unknown resource type %s for player %s."),
            *ResourceType->GetName(),
            *GetOwner()->GetName());

        return 0.0f;
    }

    return ResourceAmounts[ResourceIndex];
}

TArray<TSubclassOf<URTSResourceType>> URTSPlayerResourcesComponent::GetResourceTypes() const
{
    return ResourceTypes;
}

bool URTSPlayerResourcesComponent::CanPayResources(TSubclassOf<URTSResourceType> ResourceType, float ResourceAmount) const
{
    float AvailableResources = GetResources(ResourceType);
    return AvailableResources >= ResourceAmount;
}

bool URTSPlayerResourcesComponent::CanPayAllResources(TMap<TSubclassOf<URTSResourceType>, float> Resources) const
{
    for (auto& Resource : Resources)
    {
        if (!CanPayResources(Resource.Key, Resource.Value))
        {
            return false;
        }
    }

    return true;
}

float URTSPlayerResourcesComponent::AddResources(TSubclassOf<URTSResourceType> ResourceType, float ResourceAmount)
{
    int32 ResourceIndex = ResourceTypes.IndexOfByKey(ResourceType);

    if (ResourceIndex == INDEX_NONE)
    {
        return 0.0f;
    }

    // Get current resource amount.
    float OldResourceAmount = GetResources(ResourceType);

    // Add resources.
    float NewResourceAmount = OldResourceAmount + ResourceAmount;
    ResourceAmounts[ResourceIndex] = NewResourceAmount;

    UE_LOG(LogRTS, Log, TEXT("Player %s stock of %s has changed to %f."),
        *GetOwner()->GetName(),
        *ResourceType->GetName(),
        NewResourceAmount);

    // Notify listeners.
    OnResourcesChanged.Broadcast(ResourceType, OldResourceAmount, NewResourceAmount, false);
    return ResourceAmount;
}

float URTSPlayerResourcesComponent::PayResources(TSubclassOf<URTSResourceType> ResourceType, float ResourceAmount)
{
    // Get current resource amount.
    float OldResourceAmount = GetResources(ResourceType);

    if (OldResourceAmount < ResourceAmount)
    {
        return 0.0f;
    }

    // Deduct resources.
    return AddResources(ResourceType, -ResourceAmount);
}

void URTSPlayerResourcesComponent::PayAllResources(TMap<TSubclassOf<URTSResourceType>, float> Resources)
{
    for (auto& Resource : Resources)
    {
        PayResources(Resource.Key, Resource.Value);
    }
}

void URTSPlayerResourcesComponent::ReceivedResourceAmounts()
{
    for (int32 Index = 0; Index < ResourceTypes.Num(); ++Index)
    {
        OnResourcesChanged.Broadcast(ResourceTypes[Index], 0.0f, ResourceAmounts[Index], true);
    }
}
