#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "RTSControlGroup.generated.h"

/** Group of actors. */
USTRUCT(Category="RTS", BlueprintType)
struct REALTIMESTRATEGY_API FRTSControlGroup
{
	GENERATED_BODY()

	/** Actors of this control group. */
	UPROPERTY()
	TArray<AActor*> Actors;
};
