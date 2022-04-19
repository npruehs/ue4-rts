#pragma once

#include "CoreMinimal.h"

#include "RTSVisionActor.generated.h"


class AActor;
class URTSVisionComponent;


/** Performance optimization. Caches an actor that can update team vision, along with relevant components. */
USTRUCT()
struct REALTIMESTRATEGY_API FRTSVisionActor
{
	GENERATED_BODY()

public:
	FRTSVisionActor();
	FRTSVisionActor(AActor* InActor);

	UPROPERTY()
	AActor* Actor;

	UPROPERTY()
	URTSVisionComponent* VisionComponent;

	bool IsActorValid() const;
};
