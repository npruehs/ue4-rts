#pragma once

#include "CoreMinimal.h"

#include "Orders/RTSOrder.h"

#include "RTSSetRallyPointToLocationOrder.generated.h"


/** Orders a production actor to set its rally point to a specific target location. */
UCLASS()
class REALTIMESTRATEGY_API URTSSetRallyPointToLocationOrder : public URTSOrder
{
    GENERATED_BODY()

public:
    URTSSetRallyPointToLocationOrder(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    virtual void IssueOrder(AActor* OrderedActor, const FRTSOrderTargetData& TargetData, int32 Index) const override;
};
