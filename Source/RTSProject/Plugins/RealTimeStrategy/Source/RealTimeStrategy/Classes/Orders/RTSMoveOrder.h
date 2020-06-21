#pragma once

#include "CoreMinimal.h"

#include "Orders/RTSOrder.h"

#include "RTSMoveOrder.generated.h"


/** Orders the actor to move to the specified location. */
UCLASS()
class REALTIMESTRATEGY_API URTSMoveOrder : public URTSOrder
{
    GENERATED_BODY()

public:
    URTSMoveOrder(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
