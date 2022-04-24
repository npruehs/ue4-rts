#pragma once

#include "CoreMinimal.h"

#include "Orders/RTSOrder.h"

#include "RTSSetRallyPointToLocationOrder.generated.h"


/** Orders a production actor to set its rally point to a specific target location. */
UCLASS(Category="RTS")
class REALTIMESTRATEGY_API URTSSetRallyPointToLocationOrder : public URTSOrder
{
	GENERATED_BODY()

public:
	URTSSetRallyPointToLocationOrder(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void AddOrder(AActor* OrderedActor, const FRTSOrderTargetData& TargetData, int32 Index) const override;
	virtual void SetOrder(AActor* OrderedActor, const FRTSOrderTargetData& TargetData, int32 Index) const override;
};
