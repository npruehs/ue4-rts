#pragma once

#include "CoreMinimal.h"

#include "Orders/RTSOrder.h"

#include "RTSStopOrder.generated.h"


/** Orders the actor to stop whatever they're currently doing. */
UCLASS()
class REALTIMESTRATEGY_API URTSStopOrder : public URTSOrder
{
	GENERATED_BODY()

public:
	URTSStopOrder(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
