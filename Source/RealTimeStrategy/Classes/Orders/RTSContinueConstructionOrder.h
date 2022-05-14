#pragma once

#include "CoreMinimal.h"

#include "Orders/RTSOrder.h"

#include "RTSContinueConstructionOrder.generated.h"


/** Orders the actor to continue constructing a building. */
UCLASS(Category="RTS")
class REALTIMESTRATEGY_API URTSContinueConstructionOrder : public URTSOrder
{
	GENERATED_BODY()

public:
	URTSContinueConstructionOrder(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
