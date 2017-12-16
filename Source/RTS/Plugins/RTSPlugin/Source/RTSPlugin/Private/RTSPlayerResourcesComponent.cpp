#include "RTSPluginPCH.h"
#include "RTSPlayerResourcesComponent.h"

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

bool URTSPlayerResourcesComponent::GetResources(TSubclassOf<URTSResourceType> ResourceType, float* OutResourceAmount)
{
    // Get current resource amount.
    int32 ResourceIndex = ResourceTypes.IndexOfByKey(ResourceType);

    if (ResourceIndex == INDEX_NONE)
    {
        UE_LOG(LogRTS, Error, TEXT("Unknown resource type %s for player %s."),
            *ResourceType->GetName(),
            *GetOwner()->GetName());

        *OutResourceAmount = 0.0f;
        return false;
    }

    *OutResourceAmount = ResourceAmounts[ResourceIndex];
    return true;
}

bool URTSPlayerResourcesComponent::CanPayResources(TSubclassOf<URTSResourceType> ResourceType, float ResourceAmount)
{
    float AvailableResources;

    if (!GetResources(ResourceType, &AvailableResources))
    {
        return false;
    }

    if (AvailableResources < ResourceAmount)
    {
        return false;
    }

    return true;
}

bool URTSPlayerResourcesComponent::CanPayAllResources(TMap<TSubclassOf<URTSResourceType>, float> Resources)
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
    // Get current resource amount.
    float OldResourceAmount;
    if (!GetResources(ResourceType, &OldResourceAmount))
    {
        return 0.0f;
    }

    // Add resources.
    int32 ResourceIndex = ResourceTypes.IndexOfByKey(ResourceType);
    float NewResourceAmount = OldResourceAmount + ResourceAmount;
    ResourceAmounts[ResourceIndex] = NewResourceAmount;

    UE_LOG(LogRTS, Log, TEXT("Player %s stock of %s has changed to %f."),
        *GetOwner()->GetName(),
        *ResourceType->GetName(),
        NewResourceAmount);

    // Notify listeners.
    OnResourcesChanged.Broadcast(ResourceType, NewResourceAmount);
    return ResourceAmount;
}

float URTSPlayerResourcesComponent::PayResources(TSubclassOf<URTSResourceType> ResourceType, float ResourceAmount)
{
    // Get current resource amount.
    float OldResourceAmount;
    if (!GetResources(ResourceType, &OldResourceAmount))
    {
        return 0.0f;
    }

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
        OnResourcesChanged.Broadcast(ResourceTypes[Index], ResourceAmounts[Index]);
    }
}
