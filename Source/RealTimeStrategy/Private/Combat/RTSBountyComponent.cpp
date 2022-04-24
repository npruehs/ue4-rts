#include "Combat/RTSBountyComponent.h"

#include "GameFramework/Actor.h"
#include "GameFramework/Controller.h"

#include "RTSLog.h"
#include "RTSPlayerAIController.h"
#include "RTSPlayerState.h"
#include "Combat/RTSCombatComponent.h"
#include "Combat/RTSHealthComponent.h"
#include "Economy/RTSPlayerResourcesComponent.h"

void URTSBountyComponent::BeginPlay()
{
	Super::BeginPlay();

	const AActor* Owner = GetOwner();

	if (!IsValid(Owner))
	{
		return;
	}

	URTSCombatComponent* CombatComponent = Owner->FindComponentByClass<URTSCombatComponent>();

	if (IsValid(CombatComponent))
	{
		CombatComponent->OnKilled.AddDynamic(this, &URTSBountyComponent::OnKilled);
	}
	else
	{
		URTSHealthComponent* HealthComponent = Owner->FindComponentByClass<URTSHealthComponent>();

		if (IsValid(HealthComponent))
		{
			HealthComponent->OnKilled.AddDynamic(this, &URTSBountyComponent::OnKilled);
		}
	}
}

void URTSBountyComponent::OnKilled(AActor* Actor, AController* PreviousOwner, AActor* DamageCauser)
{
	AActor* Owner = GetOwner();

	if (!IsValid(Owner))
	{
		return;
	}

	if (!IsValid(PreviousOwner))
	{
		return;
	}

	if (!GivesBounty(PreviousOwner))
	{
		return;
	}

	if (!IsValid(DamageCauser))
	{
		return;
	}

	AController* DamageCausingPlayer = Cast<AController>(DamageCauser->GetOwner());

	if (!IsValid(DamageCausingPlayer))
	{
		return;
	}

	URTSPlayerResourcesComponent* PlayerResourcesComponent =
		DamageCausingPlayer->FindComponentByClass<URTSPlayerResourcesComponent>();

	if (!IsValid(PlayerResourcesComponent))
	{
		return;
	}

	for (const auto& Resource : Bounties)
	{
		const TSubclassOf<URTSResourceType> ResourceType = Resource.Key;
		float ResourceAmount = Resource.Value;

		PlayerResourcesComponent->AddResources(ResourceType, ResourceAmount);

		UE_LOG(LogRTS, Log, TEXT("%s collected %f %s bounty from %s."), *DamageCausingPlayer->GetName(), ResourceAmount, *ResourceType->GetName(),
		       *Actor->GetName());

		// Notify listeners.
		OnBountyCollected.Broadcast(Owner, DamageCausingPlayer, ResourceType, ResourceAmount);
	}
}

bool URTSBountyComponent::GivesBounty(AController* Player)
{
	const ARTSPlayerAIController* PlayerAIController = Cast<ARTSPlayerAIController>(Player);
	return IsValid(PlayerAIController) && PlayerAIController->GivesBounty();
}
