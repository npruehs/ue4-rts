// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GEMeleeDamage.h"
#include "Abilities/GameplayAbility.h"
#include "RTSMeleeAttackAbility.generated.h"

/**
 * 
 */
UCLASS(Category="RTS")
class REALTIMESTRATEGY_API URTSMeleeAttackAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	explicit URTSMeleeAttackAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> EffectClass = UGEMeleeDamage::StaticClass();
};
