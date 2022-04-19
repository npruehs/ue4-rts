#pragma once

#include "CoreMinimal.h"

#include "Orders/RTSOrder.h"

#include "RTSSetRallyPointToActorOrder.generated.h"


/** Orders a production actor to set its rally point to a specific target actor. */
UCLASS()
class REALTIMESTRATEGY_API URTSSetRallyPointToActorOrder : public URTSOrder
{
    GENERATED_BODY()

public:
    URTSSetRallyPointToActorOrder(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    virtual void IssueOrder(AActor* OrderedActor, const FRTSOrderTargetData& TargetData, int32 Index) const override;
};
