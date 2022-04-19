#pragma once

#include "CoreMinimal.h"

#include "RTSVisionTile.generated.h"


class AActor;


/** Tile of the vision grid. */
USTRUCT(BlueprintType)
struct REALTIMESTRATEGY_API FRTSVisionTile
{
	GENERATED_BODY()

public:
	/** Adds the specified actor as having vision on this tile. */
	void AddActor(AActor* Actor);

	/** Removes the specified actor from having vision on this tile. */
	void RemoveActor(AActor* Actor);

	/** Checks whether any actor has vision on this tile. */
	bool IsVisible() const;

private:
	/** Number of actors seeing this tile. */
	UPROPERTY()
	int32 NumActors = 0;
};
