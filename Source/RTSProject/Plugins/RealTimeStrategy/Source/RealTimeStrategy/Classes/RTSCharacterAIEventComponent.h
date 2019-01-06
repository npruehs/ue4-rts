#pragma once

#include "RealTimeStrategyPCH.h"

#include "Components/ActorComponent.h"

#include "RTSOrderType.h"

#include "RTSCharacterAIEventComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRTSCharacterAIEventComponentOrderChangedSignature, ERTSOrderType, NewOrder);


/**
* Allows interested listeners to register for AI events (e.g. issued orders).
*/
UCLASS(meta = (BlueprintSpawnableComponent))
class REALTIMESTRATEGY_API URTSCharacterAIEventComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    /** Event when the actor has received a new order. */
    UPROPERTY(BlueprintAssignable, Category = "RTS")
    FRTSCharacterAIEventComponentOrderChangedSignature OnOrderChanged;
};
