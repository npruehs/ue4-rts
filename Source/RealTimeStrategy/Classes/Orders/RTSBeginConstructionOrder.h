#pragma once

#include "CoreMinimal.h"

#include "Orders/RTSOrder.h"

#include "RTSBeginConstructionOrder.generated.h"


/** Orders the actor to start constructing a building. */
UCLASS()
class REALTIMESTRATEGY_API URTSBeginConstructionOrder : public URTSOrder
{
	GENERATED_BODY()

public:
	URTSBeginConstructionOrder(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
