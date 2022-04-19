#include "RTSContainerComponent.h"

#include "GameFramework/Actor.h"

#include "RTSContainableComponent.h"
#include "RTSLog.h"
#include "Combat/RTSHealthComponent.h"
#include "Vision/RTSVisibleComponent.h"


URTSContainerComponent::URTSContainerComponent(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
	// Set reasonable default values.
	Capacity = 1;
}

void URTSContainerComponent::BeginPlay()
{
	Super::BeginPlay();

	const AActor* Owner = GetOwner();

	if (Owner == nullptr)
	{
		return;
	}

	URTSHealthComponent* HealthComponent = Owner->FindComponentByClass<URTSHealthComponent>();

	if (HealthComponent == nullptr)
	{
		return;
	}

	HealthComponent->OnKilled.AddDynamic(this, &URTSContainerComponent::OnKilled);
}

bool URTSContainerComponent::ContainsActor(const AActor* Actor) const
{
	return ContainedActors.Contains(Actor);
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

	URTSContainableComponent* ContainableComponent = Actor->FindComponentByClass<URTSContainableComponent>();

	if (IsValid(ContainableComponent))
	{
		ContainableComponent->SetContainer(GetOwner());
	}

	// Hide actor.
	const URTSVisibleComponent* VisibleComponent = Actor->FindComponentByClass<URTSVisibleComponent>();

	if (!IsValid(VisibleComponent))
	{
		UE_LOG(LogRTS, Warning, TEXT("%s has a RTSContainerComponent, but no RTSVisibleComponent. "
			       "Actor visibility will be handled by container component itself. "
			       "Add an RTSVisibleComponent and URTSContainableComponent to ensure visibility can be determined by multiple reasons (e.g. containers and fog of war)."),
		       *Actor->GetName());
		Actor->SetActorHiddenInGame(true);
	}

	Actor->SetActorEnableCollision(false);

	// Notify listeners.
	OnActorEntered.Broadcast(GetOwner(), Actor);

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

	URTSContainableComponent* ContainableComponent = Actor->FindComponentByClass<URTSContainableComponent>();

	if (IsValid(ContainableComponent))
	{
		ContainableComponent->SetContainer(nullptr);
	}

	// Show actor.
	const URTSVisibleComponent* VisibleComponent = Actor->FindComponentByClass<URTSVisibleComponent>();

	if (!IsValid(VisibleComponent))
	{
		Actor->SetActorHiddenInGame(false);
	}

	Actor->SetActorEnableCollision(true);

	// Notify listeners.
	OnActorLeft.Broadcast(GetOwner(), Actor);

	UE_LOG(LogRTS, Log, TEXT("Actor %s has left %s."), *Actor->GetName(), *GetOwner()->GetName());
}

void URTSContainerComponent::UnloadAll()
{
	TArray<AActor*> ActorsToUnload(ContainedActors);

	for (AActor* Actor : ActorsToUnload)
	{
		UnloadActor(Actor);
	}
}

int32 URTSContainerComponent::GetCapacity() const
{
	return Capacity;
}

void URTSContainerComponent::SetCapacity(int32 InCapacity)
{
	Capacity = InCapacity;
}

TArray<AActor*> URTSContainerComponent::GetContainedActors() const
{
	return ContainedActors;
}

void URTSContainerComponent::OnKilled(AActor* Actor, AController* PreviousOwner, AActor* DamageCauser)
{
	UnloadAll();
}
