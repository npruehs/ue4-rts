#pragma once

#include "RTSPluginPrivatePCH.h"

#include "Components/ActorComponent.h"

#include "RTSAttackData.h"

#include "RTSAttackComponent.generated.h"


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
};
