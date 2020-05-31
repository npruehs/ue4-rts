#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "Templates/SubclassOf.h"

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
	ARTSProjectile(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


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

    UPROPERTY()
	AActor* Target;

	float Damage;
	TSubclassOf<class UDamageType> DamageType;

    UPROPERTY()
	AController* EventInstigator;

    UPROPERTY()
	AActor* DamageCauser;

    float TimeToImpact;

	UPROPERTY(VisibleAnywhere, Category = "RTS")
	UProjectileMovementComponent* ProjectileMovement;
};
