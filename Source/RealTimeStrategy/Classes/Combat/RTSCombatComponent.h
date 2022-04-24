// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "RTSActorDeathType.h"
#include "RTSAttackData.h"
#include "RTSAttributeSet.h"
#include "RTSGameplayTagsProvider.h"
#include "RTSCombatComponent.generated.h"

class AActor;
class USoundCue;
class ARTSAbilityProjectile;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FRTSCombatComponentKilledSignature, AActor*, Actor, AController*, PreviousOwner, AActor*, DamageCauser);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FRTSCombatComponentAttackUsedSignature, AActor*, Actor, const AActor*, Target, ARTSAbilityProjectile*, Projectile);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FRTSCombatComponentHealthChanegedSignature, AActor*, Actor, float, OldHealth, float, NewHealth, AActor*, DamageCauser);

/**
 * 
 */
UCLASS(Category = "RTS")
class REALTIMESTRATEGY_API URTSCombatComponent : public UAbilitySystemComponent, public IRTSGameplayTagsProvider
{
	GENERATED_BODY()
public:
	explicit URTSCombatComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** Uses the passed attack on the specified target and starts the cooldown timer. */
	UFUNCTION(BlueprintCallable)
	void UseAttack(int32 AttackIndex, AActor* Target);

	/** Gets the radius in which the actor will automatically select and attack targets, in cm. */
	UFUNCTION(BlueprintPure)
	float GetAcquisitionRadius() const { return AcquisitionRadius; }

	/** Gets the radius around the home location of the actor it won't leave when automatically attacking targets, in cm. */
	UFUNCTION(BlueprintPure)
	float GetChaseRadius() const { return ChaseRadius; }

	/** Gets the attack-range for the provided actor as a target. */
	UFUNCTION(BlueprintPure)
	virtual float GetAttackRange(AActor* Target) const
	{
		/* TODO concept for a per actor range check. Maybe based on damage-type or flying/grounded */
		return Attacks[0].Range;
	}

	/** Gets the attacks available for the actor. Different attacks might be used at different ranges, or against different types of targets. */
	UFUNCTION(BlueprintPure)
	TArray<FRTSAttackData> GetAttacks() const { return Attacks; }

	virtual void BeginPlay() override;

	virtual void AddGameplayTags(FGameplayTagContainer& InOutTagContainer) override;

	/** Gets the maximum health of the actor. */
	UFUNCTION(BlueprintPure)
	float GetMaximumHealth() const { return AttributeSet->GetMaxHealth(); }

	/** Gets the current health of the actor. */
	UFUNCTION(BlueprintPure)
	float GetCurrentHealth() const { return AttributeSet->GetHealth(); }

	/** Gets the last time the actor has taken damage. */
	float GetLastTimeDamageTaken() const { return LastTimeDamageTaken; }

	/** Kills the actor immediately. */
	void KillActor(AActor* DamageCauser = nullptr) const;

	/** Event when the current health of the actor has changed. */
	virtual void NotifyOnHealthChanged(AActor* Actor, float OldHealth, float NewHealth, AActor* DamageCauser);
	
	/** Event when the current health of the actor has changed. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
	FRTSCombatComponentHealthChanegedSignature OnHealthChanged;

	/** Event when the actor has been killed. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
	FRTSCombatComponentKilledSignature OnKilled;

	/** Event when an actor has used an attack. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
	FRTSCombatComponentAttackUsedSignature OnAttackUsed;

protected:
	UPROPERTY(BlueprintReadOnly, Category="RTS")
	const URTSAttributeSet* AttributeSet;

	/** Whether the actor is allowed to periodically regenerate health. */
	UPROPERTY(EditDefaultsOnly, Category = "RTS")
	bool bRegenerateHealth;

	/** Health restored for the actor, per second. */
	UPROPERTY(EditDefaultsOnly, Category = "RTS", meta = (ClampMin = 0, EditCondition = "bRegenerateHealth"))
	float HealthRegenerationRate;

	/** How to handle depleted health. */
	UPROPERTY(EditDefaultsOnly, Category = "RTS")
	ERTSActorDeathType ActorDeathType;

	/** Sound to play when the actor is killed. */
	UPROPERTY(EditDefaultsOnly, Category = "RTS")
	USoundCue* DeathSound;

	/** Attacks available for the actor. Different attacks might be used at different ranges, or against different types of targets. */
	UPROPERTY(EditDefaultsOnly, Category = "RTS")
	TArray<FRTSAttackData> Attacks;

	UPROPERTY(EditDefaultsOnly, Category="RTS")
	TArray<TSubclassOf<UGameplayAbility>> Abilities;

	/** Radius in which the actor will automatically select and attack targets, in cm. */
	UPROPERTY(EditDefaultsOnly, Category = "RTS", meta = (ClampMin = 0))
	float AcquisitionRadius;

	/** Radius around the home location of the actor it won't leave when automatically attacking targets, in cm. */
	UPROPERTY(EditDefaultsOnly, Category = "RTS", meta = (ClampMin = 0))
	float ChaseRadius;

	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilitySpecs;
	
	UPROPERTY()
	float LastTimeDamageTaken;
};
