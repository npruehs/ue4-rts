#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"

#include "Combat/RTSActorDeathType.h"

#include "RTSHealthComponent.generated.h"


class AActor;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FRTSHealthComponentHealthChangedSignature, AActor*, Actor, float, OldHealth, float, NewHealth, AActor*, DamageCauser);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FRTSHealthComponentKilledSignature, AActor*, Actor, AController*, PreviousOwner, AActor*, DamageCauser);


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

    virtual void BeginPlay() override;


    /** Gets the maximum health of the actor. */
    UFUNCTION(BlueprintPure)
    float GetMaximumHealth() const;

    /** Gets the current health of the actor. */
    UFUNCTION(BlueprintPure)
    float GetCurrentHealth() const;

    /** Sets the current health of the actor directly. */
    void SetCurrentHealth(float NewHealth, AActor* DamageCauser);


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

    /** How to handle depleted health. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS")
    ERTSActorDeathType ActorDeathType;

    /** Current health of the actor. */
    UPROPERTY(Replicated)
    float CurrentHealth;

    UFUNCTION()
    void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
};
