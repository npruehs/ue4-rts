#pragma once

#include "RealTimeStrategyPCH.h"

#include "Components/ActorComponent.h"

#include "Combat/RTSAttackData.h"

#include "RTSAttackComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRTSAttackComponentCooldownReadySignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FRTSAttackComponentAttackedUsedSignature, const FRTSAttackData&, Attack, AActor*, Target, ARTSProjectile*, Projectile);


/**
* Adds one or more attacks to the actor.
* These can also be used for healing.
*/
UCLASS(meta = (BlueprintSpawnableComponent))
class REALTIMESTRATEGY_API URTSAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URTSAttackComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	/** Uses the passed attack on the specified target and starts the cooldown timer. */
	UFUNCTION(BlueprintCallable)
	void UseAttack(int32 AttackIndex, AActor* Target);

    /** Gets the radius in which the actor will automatically select and attack targets, in cm. */
    UFUNCTION(BlueprintPure)
    float GetAcquisitionRadius() const;

    /** Gets the radius around the home location of the actor it won't leave when automatically attacking targets, in cm. */
    UFUNCTION(BlueprintPure)
    float GetChaseRadius() const;

    /** Gets the attacks available for the actor. Different attacks might be used at different ranges, or against different types of targets. */
    UFUNCTION(BlueprintPure)
    TArray<FRTSAttackData> GetAttacks() const;

    /** Gets the time before the next attack can be used, in seconds. This is shared between attacks.*/
    UFUNCTION(BlueprintPure)
    float GetRemainingCooldown() const;


	/** Event when the attack cooldown has expired. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
	FRTSAttackComponentCooldownReadySignature OnCooldownReady;

	/** Event when an actor has used an attack. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
	FRTSAttackComponentAttackedUsedSignature OnAttackUsed;

private:
    /** Radius in which the actor will automatically select and attack targets, in cm. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS", meta = (ClampMin = 0))
    float AcquisitionRadius;

    /** Radius around the home location of the actor it won't leave when automatically attacking targets, in cm. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS", meta = (ClampMin = 0))
    float ChaseRadius;

    /** Attacks available for the actor. Different attacks might be used at different ranges, or against different types of targets. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS")
    TArray<FRTSAttackData> Attacks;

    /** Time before the next attack can be used, in seconds. This is shared between attacks.*/
    float RemainingCooldown;
};
