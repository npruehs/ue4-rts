#pragma once

#include "CoreMinimal.h"
#include "RTSOrderType.h"

#include "Templates/SubclassOf.h"

#include "Orders/RTSOrder.h"

#include "RTSOrderData.generated.h"


class AActor;


/** Order to be issued to an actor. */
USTRUCT(BlueprintType)
struct REALTIMESTRATEGY_API FRTSOrderData
{
	GENERATED_BODY()

	FRTSOrderData();
	ERTSOrderType GetOrderType() const;

	/** Type of the order to issue. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS")
	TSubclassOf<URTSOrder> OrderClass;

	/** Target actor. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS")
	AActor* TargetActor;

	/** Target location. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS")
	FVector TargetLocation;

	/** Additional index. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS")
	int32 Index;
};
