// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "UObject/Object.h"
#include "RTSRangedAttackAbility.generated.h"

class ARTSAbilityProjectile;
/**
 * 
 */
UCLASS(Category="RTS")
class REALTIMESTRATEGY_API URTSRangedAttackAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	explicit URTSRangedAttackAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UPROPERTY(EditAnywhere, Category="RTS")
	TSubclassOf<ARTSAbilityProjectile> ProjectileClass;
};
