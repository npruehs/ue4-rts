// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Abilities/RTSRangedAttackAbility.h"

#include "RTSLog.h"
#include "Combat/RTSAbilityProjectile.h"
#include "Combat/RTSCombatComponent.h"
#include "Combat/RTSProjectile.h"
#include "Libraries/RTSGameplayTagLibrary.h"

URTSRangedAttackAbility::URTSRangedAttackAbility(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	ActivationBlockedTags.AddTag(URTSGameplayTagLibrary::Attack_Cooldown());
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::Type::ReplicateYes;
}

void URTSRangedAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!ProjectileClass.Get())
	{
		return;
	}

	CommitAbility(Handle, ActorInfo, ActivationInfo, nullptr);

	UE_LOG(LogRTS, Log, TEXT("Actor %s attacks %s."), *ActorInfo->OwnerActor->GetName(), *TriggerEventData->Target->GetName());

	// Fire projectile.
	// Build spawn transform.
	const FVector SpawnLocation = ActorInfo->OwnerActor->GetActorLocation();
	const FRotator SpawnRotation = ActorInfo->OwnerActor->GetActorRotation();
	const FTransform SpawnTransform = FTransform(SpawnRotation, SpawnLocation);

	// Build spawn info.
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = Cast<APawn>(ActorInfo->OwnerActor);
	SpawnInfo.Owner = Cast<APawn>(ActorInfo->OwnerActor);
	SpawnInfo.ObjectFlags |= RF_Transient;

	// Spawn projectile.

	if (ARTSAbilityProjectile* SpawnedProjectile = GetWorld()->SpawnActor<ARTSAbilityProjectile>(ProjectileClass, SpawnTransform, SpawnInfo))
	{
		UE_LOG(LogRTS, Log, TEXT("%s fired projectile %s at target %s."), *ActorInfo->OwnerActor->GetName(), *SpawnedProjectile->GetName(), *TriggerEventData->Target->GetName());

		// Aim at target.
		SpawnedProjectile->SetTarget(TriggerEventData->Target);

		const URTSCombatComponent* CombatComponent = Cast<URTSCombatComponent>(ActorInfo->AbilitySystemComponent.Get());
		CombatComponent->OnAttackUsed.Broadcast(ActorInfo->OwnerActor.Get(), TriggerEventData->Target, SpawnedProjectile);
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
