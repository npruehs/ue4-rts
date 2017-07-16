#pragma once

#include "RTSPluginPrivatePCH.h"

#include "Components/ActorComponent.h"

#include "RTSHealthComponent.generated.h"


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


	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
