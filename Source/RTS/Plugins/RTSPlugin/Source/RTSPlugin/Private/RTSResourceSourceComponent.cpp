#include "RTSPluginPrivatePCH.h"
#include "RTSResourceSourceComponent.h"

#include "Net/UnrealNetwork.h"



URTSResourceSourceComponent::URTSResourceSourceComponent(const FObjectInitializer& ObjectInitializer)
{
	GatheringFactor = 1.0f;
	GathererCapacity = 1;

	SetIsReplicated(true);
}

void URTSResourceSourceComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URTSResourceSourceComponent, CurrentResources);
}

float URTSResourceSourceComponent::ExtractResources(AActor* Gatherer, float ResourceAmount)
{
	// Compute actual amount gathered.
	float GatheredAmount = ResourceAmount * GatheringFactor;

	if (GatheredAmount > CurrentResources)
	{
		GatheredAmount = CurrentResources;
	}

	// Deduct resources.
	float OldResources = CurrentResources;
	CurrentResources -= GatheredAmount;
	float NewResources = CurrentResources;

	UE_LOG(RTSLog, Log, TEXT("Actor %s has gathered %f resources of type %s from %s, reducing remaining resources to %f."),
		*Gatherer->GetName(),
		GatheredAmount,
		*ResourceType->GetName(),
		*GetOwner()->GetName(),
		CurrentResources);

	// Notify listeners.
	OnResourcesChanged.Broadcast(OldResources, NewResources);

	// Check if we're depleted.
	if (CurrentResources <= 0)
	{
		UE_LOG(RTSLog, Log, TEXT("Actor %s has been depleted."), *GetOwner()->GetName());

		// Destroy this actor.
		GetOwner()->Destroy();

		// Notify listeners.
		OnDepleted.Broadcast();
	}

	return GatheredAmount;
}
