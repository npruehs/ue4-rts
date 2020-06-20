#pragma once

#include "CoreMinimal.h"

#include "Orders/RTSOrder.h"

#include "RTSProduceOrder.generated.h"


/** Orders the actor to produce another actor. */
UCLASS()
class REALTIMESTRATEGY_API URTSProduceOrder : public URTSOrder
{
    GENERATED_BODY()

public:
    URTSProduceOrder(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
