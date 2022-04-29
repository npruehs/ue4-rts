#include "Vision/RTSVisionVolume.h"

#include "Components/BrushComponent.h"

#include "RTSLog.h"


ARTSVisionVolume::ARTSVisionVolume(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
	// Set reasonable default values.
	SizeInTiles = 256;
	LevelHeight = 250.0f;
	HeightLevelTraceChannel = ECC_WorldStatic;
}

void ARTSVisionVolume::Initialize()
{
	// Get vision world size.
	const UBrushComponent* VisionBrushComponent = GetBrushComponent();
	const FBoxSphereBounds VisionBounds = VisionBrushComponent->CalcBounds(VisionBrushComponent->GetComponentTransform());

	SizeInWorld = VisionBounds.BoxExtent * 2;

	// Calculate tile size.
	TileSize = SizeInWorld.X / SizeInTiles;

	UE_LOG(LogRTS, Log, TEXT("%s has %i tiles of world size %f."), *GetName(), SizeInTiles, TileSize);

	// Calculate tile world locations.
	TileWorldLocations.SetNum(SizeInTiles * SizeInTiles);

	for (int32 TileY = 0; TileY < SizeInTiles; ++TileY)
	{
		for (int32 TileX = 0; TileX < SizeInTiles; ++TileX)
		{
			const float RelativeWorldX = (float)TileX / SizeInTiles;
			const float RelativeWorldY = (float)TileY / SizeInTiles;

			const float TranslatedWorldPositionX = (RelativeWorldX - 0.5f) * SizeInWorld.X;
			const float TranslatedWorldPositionY = (RelativeWorldY - 0.5f) * SizeInWorld.Y;

			const FVector2D WorldPosition = FVector2D(TranslatedWorldPositionX + GetActorLocation().X,
			                                          TranslatedWorldPositionY + GetActorLocation().Y);

			TileWorldLocations[TileY * SizeInTiles + TileX] = WorldPosition;
		}
	}

	// Cache tile heights.
	TileHeights.SetNum(SizeInTiles * SizeInTiles);

	for (int32 Y = 0; Y <= SizeInTiles - 1; ++Y)
	{
		for (int32 X = 0; X <= SizeInTiles - 1; ++X)
		{
			FVector2D TileLocationWorld2D = TileToWorld(FIntVector(X, Y, 0));
			const float TileHeightWorld = CalculateWorldHeightAtLocation(TileLocationWorld2D);
			const int32 TileHeightLevel = FMath::FloorToInt(TileHeightWorld / LevelHeight);

			TileHeights[GetTileIndex(X, Y)] = TileHeightLevel;
		}
	}
}

int32 ARTSVisionVolume::GetSizeInTiles() const
{
	return SizeInTiles;
}

FVector ARTSVisionVolume::GetSizeInWorld() const
{
	return SizeInWorld;
}

float ARTSVisionVolume::GetTileSize() const
{
	return TileSize;
}

int32 ARTSVisionVolume::GetTileHeight(const FIntVector& Tile) const
{
	const int32 TileIndex = GetTileIndex(Tile.X, Tile.Y);
	return TileHeights.IsValidIndex(TileIndex) ? TileHeights[TileIndex] : 0;
}

ERTSVisionState ARTSVisionVolume::GetMinimumVisionState() const
{
	return MinimumVisionState;
}

FIntVector ARTSVisionVolume::WorldToTile(const FVector& WorldPosition) const
{
	// Get relative world position.
	const float RelativeWorldX = WorldPosition.X / SizeInWorld.X + 0.5f;
	const float RelativeWorldY = WorldPosition.Y / SizeInWorld.Y + 0.5f;

	// Convert to minimap coordinates.
	const int32 TileX = FMath::FloorToInt(RelativeWorldX * SizeInTiles);
	const int32 TileY = FMath::FloorToInt(RelativeWorldY * SizeInTiles);

	return FIntVector(TileX, TileY, 0);
}

int32 ARTSVisionVolume::GetTileIndex(int32 X, int32 Y) const
{
	return Y * SizeInTiles + X;
}

float ARTSVisionVolume::CalculateWorldHeightAtLocation(const FVector2D& WorldLocation)
{
	// Cast ray to hit world.
	TArray<FHitResult> HitResults;

	GetWorld()->LineTraceMultiByChannel(HitResults, FVector(WorldLocation.X, WorldLocation.Y, 10000.0f),
	                                    FVector(WorldLocation.X, WorldLocation.Y, -10000.0f), HeightLevelTraceChannel);

	for (auto& HitResult : HitResults)
	{
		if (HitResult.HasValidHitObjectHandle())
		{
			return HitResult.Location.Z;
		}
	}

	return 0.0f;
}
