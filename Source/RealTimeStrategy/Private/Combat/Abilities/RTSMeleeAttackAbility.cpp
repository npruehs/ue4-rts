// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Abilities/RTSMeleeAttackAbility.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

URTSMeleeAttackAbility::URTSMeleeAttackAbility(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
}

void URTSMeleeAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	AAIController* Controller = Cast<AAIController>(Cast<APawn>(ActorInfo->OwnerActor)->GetController());

	if (!IsValid(Controller))
	{
		return;
	}

	const AActor* Target = Cast<AActor>(Controller->GetBlackboardComponent()->GetValueAsObject("TargetActor"));
	if (!IsValid(Target))
	{
		return;
	}

	CommitAbility(Handle, ActorInfo, ActivationInfo, nullptr);

	if (UAbilitySystemComponent* AbilitySystemComponentFromActor = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Target); IsValid(AbilitySystemComponentFromActor))
	{
		const FGameplayEffectContextHandle Context;
		UGameplayEffect* Effect = EffectClass->GetDefaultObject<UGameplayEffect>();
		ActorInfo->AbilitySystemComponent->ApplyGameplayEffectToTarget(Effect, AbilitySystemComponentFromActor, 1, Context);
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
