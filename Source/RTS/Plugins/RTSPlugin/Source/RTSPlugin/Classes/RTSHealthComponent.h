#pragma once

#include "RTSPluginPrivatePCH.h"

#include "Components/ActorComponent.h"

#include "RTSHealthComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRTSHealthComponentHealthChangedSignature, float, OldHealth, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRTSHealthComponentKilledSignature, AController*, PreviousOwner);


/**
* Adds health to the actor, e.g. for taking damage and dying.
*/
UCLASS(meta = (BlueprintSpawnableComponent))
class URTSHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/** Current health of the actor. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS", meta = (ClampMin = 0), replicated)
	float CurrentHealth;

	/** Maximum health of the actor. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS", meta = (ClampMin = 0))
	float MaximumHealth;


	URTSHealthComponent(const FObjectInitializer& ObjectInitializer);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	/** Apply damage to this actor. */
	float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);


	/** Event when the current health of the actor has changed. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
	FRTSHealthComponentHealthChangedSignature OnHealthChanged;

	/** Event when the actor has been killed. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
	FRTSHealthComponentKilledSignature OnKilled;
};
