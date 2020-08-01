#pragma once

#include "CoreMinimal.h"

#include "RTSActorDeathType.generated.h"

/** How to handle depleted health. */
UENUM(BlueprintType)
enum class ERTSActorDeathType : uint8
{
    /** Do nothing. */
    DEATH_DoNothing,

    /** Stop gameplay for the actor, removing most of its components. */
    DEATH_StopGameplay,

    /** Destroy the actor. */
    DEATH_Destroy
};
