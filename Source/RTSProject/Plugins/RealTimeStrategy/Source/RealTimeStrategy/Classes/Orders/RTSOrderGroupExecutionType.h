#pragma once

#include "CoreMinimal.h"

#include "RTSOrderGroupExecutionType.generated.h"

/** How many and which of the selected actors an order should be issued to. */
UENUM(BlueprintType)
enum class ERTSOrderGroupExecutionType : uint8
{
    /** Order is simply issued to all selected actors. */
    ORDERGROUPEXECUTION_All,

    /** Order is issued to a single selected actor. */
    ORDERGROUPEXECUTION_Any,
};
