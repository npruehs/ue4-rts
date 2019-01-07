#pragma once

#include "RealTimeStrategyPCH.h"

#include "GameFramework/Volume.h"

#include "RTSVisionVolume.generated.h"


/**
* Volume that defines the size and resolution of vision provided by units.
*/
UCLASS(Blueprintable)
class REALTIMESTRATEGY_API ARTSVisionVolume : public AVolume
{
	GENERATED_UCLASS_BODY()

public:
    float GetSizePerTile() const;
	FVector GetWorldSize() const;
	FIntVector GetTileSize() const;
    
	FIntVector WorldToTile(const FVector& WorldPosition) const;

protected:
	virtual void BeginPlay() override;

private:
    /** Size of a single tile of the vision grid imposed on the world. */
    UPROPERTY(EditInstanceOnly, Category = "RTS", meta = (ClampMin = 0))
    float SizePerTile;

	FVector WorldSize;
	FIntVector TileSize;
};
