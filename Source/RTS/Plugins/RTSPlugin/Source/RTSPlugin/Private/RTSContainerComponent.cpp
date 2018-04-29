#include "RTSPluginPCH.h"
#include "RTSContainerComponent.h"

#include "GameFramework/Actor.h"


URTSContainerComponent::URTSContainerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set reasonable default values.
	Capacity = 1;
}

bool URTSContainerComponent::CanLoadActor(AActor* Actor) const
{
	return Capacity < 0 || ContainedActors.Num() < Capacity;
}

void URTSContainerComponent::LoadActor(AActor* Actor)
{
	if (!CanLoadActor(Actor))
	{
		return;
	}

	if (ContainedActors.Contains(Actor))
	{
		return;
	}

	// Add to container.
	ContainedActors.Add(Actor);

	// Hide actor.
	Actor->SetActorHiddenInGame(true);
	Actor->SetActorEnableCollision(false);

	// Notify listeners.
	OnActorEntered.Broadcast(Actor);

	UE_LOG(LogRTS, Log, TEXT("Actor %s has entered %s."), *Actor->GetName(), *GetOwner()->GetName());
}

void URTSContainerComponent::UnloadActor(AActor* Actor)
{
	if (!ContainedActors.Contains(Actor))
	{
		return;
	}

	// Remove from container.
	ContainedActors.Remove(Actor);

	// Show actor.
	Actor->SetActorHiddenInGame(false);
	Actor->SetActorEnableCollision(true);

	// Notify listeners.
	OnActorLeft.Broadcast(Actor);

	UE_LOG(LogRTS, Log, TEXT("Actor %s has left %s."), *Actor->GetName(), *GetOwner()->GetName());
}
