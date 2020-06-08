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
    /** Whether the projectile should follow a ballistic trajectory on its way. Should not be used with homing projectiles. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS")
    bool bBallisticTrajectory;

    /** How much to exaggerate the ballistic trajectory. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS", meta = (EditCondition = bBallisticTrajectory))
    float BallisticTrajectoryFactor;

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

    /** How far away the projectile started flying towards its target. */
    float InitialDistance;

    /** How far above the ground the projectile started flying towards its target. */
    float InitialHeight;

    /** How far above the ground the target was when the projectile started flying towards it. */
    float TargetHeight;

    /** Angle at which the projectile has been launched if following a ballistic trajectory. */
    float LaunchAngle;

	UPROPERTY(VisibleAnywhere, Category = "RTS")
	UProjectileMovementComponent* ProjectileMovement;

    UFUNCTION(NetMulticast, Reliable)
    void MulticastFireAt(AActor* ProjectileTarget,
            float ProjectileDamage,
            TSubclassOf<class UDamageType> ProjectileDamageType,
            AController* ProjectileEventInstigator,
            AActor* ProjectileDamageCauser);
};
