#include "Vision/RTSVisionVolume.h"

#include "Components/BrushComponent.h"

#include "RTSLog.h"


ARTSVisionVolume::ARTSVisionVolume(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
	// Set reasonable default values.
	SizeInTiles = 256;
}

void ARTSVisionVolume::Initialize()
{
    // Get vision world size.
    UBrushComponent* VisionBrushComponent = GetBrushComponent();
    FBoxSphereBounds VisionBounds = VisionBrushComponent->CalcBounds(VisionBrushComponent->GetComponentTransform());

    SizeInWorld = VisionBounds.BoxExtent * 2;

    // Calculate tile size.
    TileSize = SizeInWorld.X / SizeInTiles;

    UE_LOG(LogRTS, Log, TEXT("%s has %i tiles of world size %f."), *GetName(), SizeInTiles, TileSize);
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

ERTSVisionState ARTSVisionVolume::GetMinimumVisionState() const
{
    return MinimumVisionState;
}

FIntVector ARTSVisionVolume::WorldToTile(const FVector& WorldPosition) const
{
	// Get relative world position.
	float RelativeWorldX = WorldPosition.X / SizeInWorld.X + 0.5f;
	float RelativeWorldY = WorldPosition.Y / SizeInWorld.Y + 0.5f;

	// Convert to minimap coordinates.
	int32 TileX = FMath::FloorToInt(RelativeWorldX * SizeInTiles);
	int32 TileY = FMath::FloorToInt(RelativeWorldY * SizeInTiles);

	return FIntVector(TileX, TileY, 0);
}
