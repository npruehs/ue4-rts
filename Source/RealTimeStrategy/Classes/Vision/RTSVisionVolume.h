#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Volume.h"

#include "Vision/RTSVisionState.h"

#include "RTSVisionVolume.generated.h"


/**
* Volume that defines the size and resolution of vision provided by units.
*/
UCLASS(Blueprintable)
class REALTIMESTRATEGY_API ARTSVisionVolume : public AVolume
{
	GENERATED_BODY()

public:
	ARTSVisionVolume(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** Prepares this actor for gameplay. */
	void Initialize();

	/** Gets the width and height of the vision grid imposed on the world. */
	int32 GetSizeInTiles() const;

	/** Gets the size of the vision volume, in world space. */
	FVector GetSizeInWorld() const;

	/** Gets the width and height of a single vision grid tile, in cm. */
	float GetTileSize() const;

	/** Gets the height level of the specified vision grid tile. */
	int32 GetTileHeight(const FIntVector& Tile) const;

	/** Gets the minimum vision state of the world. */
	ERTSVisionState GetMinimumVisionState() const;

	inline FVector2D TileToWorld(const FIntVector& Tile) const
	{
		return TileToWorldXY(Tile.X, Tile.Y);
	}

	inline FVector2D TileToWorldXY(int32 TileX, int32 TileY) const
	{
		return TileWorldLocations[TileY * SizeInTiles + TileX];
	}

	FIntVector WorldToTile(const FVector& WorldPosition) const;

private:
	/** Width and height of the vision grid imposed on the world. */
	UPROPERTY(EditAnywhere, Category = "RTS")
	int32 SizeInTiles;

	/** Height of a height level imposed on the world. */
	UPROPERTY(EditAnywhere, Category = "RTS", meta = (ClampMin = 0))
	float LevelHeight;

	/** Collision channel to trace for when determining tile heights. */
	UPROPERTY(EditAnywhere, Category = "RTS")
	TEnumAsByte<ECollisionChannel> HeightLevelTraceChannel;

	/** Minimum vision state of the world. Change this for removing the black part of fog of war, or disabling it entirely. */
	UPROPERTY(EditAnywhere, Category = "RTS")
	ERTSVisionState MinimumVisionState;

	/** Size of the vision volume, in world space. */
	FVector SizeInWorld;

	/** Width and height of a single vision grid tile, in cm. */
	float TileSize;

	/** Cached world locations of all vision grid tiles. */
	TArray<FVector2D> TileWorldLocations;

	/** Cached height levels of all vision grid tiles. */
	TArray<int32> TileHeights;

	/** Gets the index of the tile with the specified coordinates. */
	int32 GetTileIndex(int32 X, int32 Y) const;

	/** Traces the height of the specified world location. */
	float CalculateWorldHeightAtLocation(const FVector2D& WorldLocation);
};
