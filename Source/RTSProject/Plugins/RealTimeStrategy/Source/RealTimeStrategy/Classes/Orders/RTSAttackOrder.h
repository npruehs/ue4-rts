#pragma once

#include "CoreMinimal.h"

#include "Orders/RTSOrder.h"

#include "RTSAttackOrder.generated.h"


/** Orders the actor to attack another actor. */
UCLASS()
class REALTIMESTRATEGY_API URTSAttackOrder : public URTSOrder
{
    GENERATED_BODY()

public:
    URTSAttackOrder(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
