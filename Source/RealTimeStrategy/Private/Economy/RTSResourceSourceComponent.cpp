#include "Economy/RTSResourceSourceComponent.h"

#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"

#include "RTSContainerComponent.h"
#include "RTSGameplayTagsComponent.h"
#include "RTSLog.h"
#include "Libraries/RTSGameplayTagLibrary.h"


URTSResourceSourceComponent::URTSResourceSourceComponent(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(true);

	// Set reasonable default values.
	MaximumResources = 1000.0f;
	GatheringFactor = 1.0f;
	GathererCapacity = 1;

	InitialGameplayTags.AddTag(URTSGameplayTagLibrary::Status_Permanent_CanBeGathered());
}

void URTSResourceSourceComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URTSResourceSourceComponent, CurrentResources);
}

void URTSResourceSourceComponent::BeginPlay()
{
	Super::BeginPlay();

	URTSGameplayTagsComponent* GameplayTagsComponent = GetOwner()->FindComponentByClass<URTSGameplayTagsComponent>();

	if (!IsValid(GameplayTagsComponent))
	{
		UE_LOG(LogRTS, Warning, TEXT("%s does not have an RTSGameplayTagsComponent attached. This will be required in future releases. Please add the component now to prevent issues later."),
		       *GetOwner()->GetName());
	}

	// Set container size.
	const auto ContainerComponent = GetOwner()->FindComponentByClass<URTSContainerComponent>();

	if (ContainerComponent)
	{
		ContainerComponent->SetCapacity(GathererCapacity);

		if (IsValid(GameplayTagsComponent))
		{
			GameplayTagsComponent->AddGameplayTag(URTSGameplayTagLibrary::Container_ResourceSource());
		}
	}

	// Set initial resources.
	CurrentResources = MaximumResources;
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
	const float OldResources = CurrentResources;
	CurrentResources -= GatheredAmount;
	const float NewResources = CurrentResources;

	UE_LOG(LogRTS, Log, TEXT("Actor %s has gathered %f resources of type %s from %s, reducing remaining resources to %f."),
	       *Gatherer->GetName(),
	       GatheredAmount,
	       *ResourceType->GetName(),
	       *GetOwner()->GetName(),
	       CurrentResources);

	// Notify listeners.
	OnResourcesChanged.Broadcast(GetOwner(), OldResources, NewResources);

	// Check if we're depleted.
	if (CurrentResources <= 0)
	{
		UE_LOG(LogRTS, Log, TEXT("Actor %s has been depleted."), *GetOwner()->GetName());

		// Notify listeners.
		OnDepleted.Broadcast(GetOwner());

		// Destroy this actor.
		GetOwner()->Destroy();
	}

	return GatheredAmount;
}

bool URTSResourceSourceComponent::CanGathererEnter(AActor* Gatherer) const
{
	const auto ContainerComponent = GetOwner()->FindComponentByClass<URTSContainerComponent>();
	return !ContainerComponent || ContainerComponent->GetContainedActors().Contains(Gatherer) || ContainerComponent->CanLoadActor(Gatherer);
}

TSubclassOf<URTSResourceType> URTSResourceSourceComponent::GetResourceType() const
{
	return ResourceType;
}

float URTSResourceSourceComponent::GetMaximumResources() const
{
	return MaximumResources;
}

float URTSResourceSourceComponent::GetGatheringFactor() const
{
	return GatheringFactor;
}

bool URTSResourceSourceComponent::MustGathererEnter() const
{
	return bGathererMustEnter;
}

int32 URTSResourceSourceComponent::GetGathererCapacity() const
{
	return GathererCapacity;
}

float URTSResourceSourceComponent::GetCurrentResources() const
{
	return CurrentResources;
}
