#pragma once

#include "CoreMinimal.h"

#include "Orders/RTSOrder.h"

#include "RTSGatherOrder.generated.h"


/** Orders the actor to gather resources from another actor. */
UCLASS()
class REALTIMESTRATEGY_API URTSGatherOrder : public URTSOrder
{
	GENERATED_BODY()

public:
	URTSGatherOrder(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual bool IsValidTarget(const AActor* OrderedActor, const FRTSOrderTargetData& TargetData, int32 Index) const override;
};
