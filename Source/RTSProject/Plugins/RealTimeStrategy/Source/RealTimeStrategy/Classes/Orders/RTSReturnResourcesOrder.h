#pragma once

#include "CoreMinimal.h"

#include "Orders/RTSOrder.h"

#include "RTSReturnResourcesOrder.generated.h"


/** Orders the actor to return resources to another actor. */
UCLASS()
class REALTIMESTRATEGY_API URTSReturnResourcesOrder : public URTSOrder
{
    GENERATED_BODY()

public:
    URTSReturnResourcesOrder(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    virtual bool IsValidTarget(const AActor* OrderedActor, const FRTSOrderTargetData& TargetData, int32 Index) const override;
};
