#pragma once

#include "DaeTestComparisonMethod.generated.h"

UENUM(BlueprintType)
enum class EDaeTestComparisonMethod : uint8
{
    LessThan,
    LessThanOrEqualTo,
    GreaterThanOrEqualTo,
    GreaterThan
};
