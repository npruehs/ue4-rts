#pragma once

#include "RTSPluginPrivatePCH.h"

#include "Components/ActorComponent.h"

#include "RTSAttackData.h"

#include "RTSAttackComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRTSAttackComponentCooldownReadySignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FRTSAttackComponentAttackedUsedSignature, const FRTSAttackData&, Attack, AActor*, Target, ARTSProjectile*, Projectile);


/**
* Adds one or more attacks to the actor.
* These can also be used for healing.
*/
UCLASS(meta = (BlueprintSpawnableComponent))
class URTSAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/** Radius in which the actor will automatically select and attack targets, in cm. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS")
	float AcquisitionRadius;

	/** Radius around the home location of the actor it won't leave when automatically attacking targets, in cm. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS")
	float ChaseRadius;

	/** Attacks available for the actor. Different attacks might be used at different ranges, or against different types of targets. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS")
	TArray<FRTSAttackData> Attacks;

	/** Time before the next attack can be used, in seconds. This is shared between attacks.*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS")
	float RemainingCooldown;


	URTSAttackComponent(const FObjectInitializer& ObjectInitializer);


	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;


	/** Uses the passed attack on the specified target and starts the cooldown timer. */
	UFUNCTION(BlueprintCallable)
	void UseAttack(int AttackIndex, AActor* Target);


	/** Event when the attack cooldown has expired. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
	FRTSAttackComponentCooldownReadySignature OnCooldownReady;

	/** Event when a character has used an attack. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
	FRTSAttackComponentAttackedUsedSignature OnAttackUsed;
};
