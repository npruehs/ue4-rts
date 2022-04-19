#pragma once

#include "CoreMinimal.h"

#include "RTSVisibleActor.generated.h"


class AActor;
class URTSVisibleComponent;
class URTSVisionComponent;
class URTSOwnerComponent;


/** Performance optimization. Caches an actor that can be visible or hidden, along with relevant components. */
USTRUCT()
struct REALTIMESTRATEGY_API FRTSVisibleActor
{
	GENERATED_BODY()

public:
	FRTSVisibleActor();
	FRTSVisibleActor(AActor* InActor);

	UPROPERTY()
	AActor* Actor;

	UPROPERTY()
	URTSOwnerComponent* OwnerComponent;

	UPROPERTY()
	URTSVisibleComponent* VisibleComponent;

	UPROPERTY()
	URTSVisionComponent* VisionComponent;

	bool IsActorValid() const;
};
