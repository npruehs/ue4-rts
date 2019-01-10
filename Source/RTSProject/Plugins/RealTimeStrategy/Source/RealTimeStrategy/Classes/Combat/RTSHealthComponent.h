#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"

#include "RTSHealthComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FRTSHealthComponentHealthChangedSignature, float, OldHealth, float, NewHealth, AActor*, DamageCauser);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRTSHealthComponentKilledSignature, AController*, PreviousOwner, AActor*, DamageCauser);


/**
* Adds health to the actor, e.g. for taking damage and dying.
*/
UCLASS(meta = (BlueprintSpawnableComponent))
class REALTIMESTRATEGY_API URTSHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URTSHealthComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	/** Apply damage to this actor. */
	float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);


    /** Gets the maximum health of the actor. */
    UFUNCTION(BlueprintPure)
    float GetMaximumHealth() const;

    /** Gets the current health of the actor. */
    UFUNCTION(BlueprintPure)
    float GetCurrentHealth() const;


	/** Event when the current health of the actor has changed. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
	FRTSHealthComponentHealthChangedSignature OnHealthChanged;

	/** Event when the actor has been killed. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
	FRTSHealthComponentKilledSignature OnKilled;

private:
    /** Maximum health of the actor. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS", meta = (ClampMin = 0))
    float MaximumHealth;

    /** Current health of the actor. */
    UPROPERTY(replicated)
    float CurrentHealth;
};
