#pragma once

#include "RealTimeStrategyPCH.h"

#include "GameFramework/Actor.h"

#include "RTSProjectile.generated.h"


class UProjectileMovementComponent;


/**
* Projectile with RTS features, such as following a target and dealing damage.
*/
UCLASS()
class REALTIMESTRATEGY_API ARTSProjectile : public AActor
{
	GENERATED_BODY()

public:
	ARTSProjectile();


	/** Locks on to the specified target, dealing damage on impact. */
	UFUNCTION(BlueprintCallable)
	void FireAt(
		AActor* ProjectileTarget,
		float ProjectileDamage,
		TSubclassOf<class UDamageType> ProjectileDamageType,
		AController* ProjectileInstigator,
		AActor* ProjectileDamageCauser);

	virtual void Tick(float DeltaSeconds) override;


	/** Event when this projectile hits its target. */
	virtual void NotifyOnProjectileDetonated(
		AActor* ProjectileTarget,
		float ProjectileDamage,
		TSubclassOf<class UDamageType> ProjectileDamageType,
		AController* ProjectileInstigator,
		AActor* ProjectileDamageCauser);

	/** Event when this projectile hits its target. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS", meta = (DisplayName = "OnProjectileDetonated"))
	void ReceiveOnProjectileDetonated(
		AActor* ProjectileTarget,
		float ProjectileDamage,
		TSubclassOf<class UDamageType> ProjectileDamageType,
		AController* ProjectileInstigator,
		AActor* ProjectileDamageCauser);

private:
    bool bFired;
	AActor* Target;
	float Damage;
	TSubclassOf<class UDamageType> DamageType;
	AController* EventInstigator;
	AActor* DamageCauser;
    float TimeToImpact;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;
};
