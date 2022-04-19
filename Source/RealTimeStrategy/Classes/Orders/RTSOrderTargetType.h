#pragma once

#include "CoreMinimal.h"

#include "RTSOrderTargetType.generated.h"


/** Possible target types of an order. */
UENUM(BlueprintType)
enum class ERTSOrderTargetType : uint8
{
    /** No specific target (e.g. aura or automatically applied to self). */
    ORDERTARGET_None,

    /** Order needs an actor as target. */
    ORDERTARGET_Actor,

    /** Order needs a vector location as target. */
    ORDERTARGET_Location
};
