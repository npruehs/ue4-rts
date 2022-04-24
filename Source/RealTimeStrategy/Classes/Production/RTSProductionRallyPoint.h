#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "RTSProductionRallyPoint.generated.h"


/** Actor or location to send new actors to. */
USTRUCT(Category="RTS", BlueprintType)
struct REALTIMESTRATEGY_API FRTSProductionRallyPoint
{
	GENERATED_BODY()

	/** Location to send new actors to. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS")
	FVector TargetLocation;

	/** Actor to send new actors to. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS")
	AActor* TargetActor;

	/** Whether this rally is currently active. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS")
	bool bIsSet;
};
