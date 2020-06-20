#pragma once

#include "CoreMinimal.h"

#include "Templates/SubclassOf.h"

#include "Orders/RTSOrder.h"

#include "RTSOrderData.generated.h"


class AActor;


/** Order to be issued to an actor. */
USTRUCT(BlueprintType)
struct REALTIMESTRATEGY_API FRTSOrderData
{
    GENERATED_BODY()

public:
    /** Type of the order to issue. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS")
    TSubclassOf<URTSOrder> OrderClass;

    /** Target actor. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS")
    AActor* TargetActor;

    /** Target location. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS")
    FVector TargetLocation;

    /** Additional index. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS")
    int32 Index;
};
