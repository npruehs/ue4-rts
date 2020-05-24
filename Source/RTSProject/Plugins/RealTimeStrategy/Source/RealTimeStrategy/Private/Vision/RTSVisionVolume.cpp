#include "Vision/RTSVisionVolume.h"

#include "Components/BrushComponent.h"

#include "RTSLog.h"


ARTSVisionVolume::ARTSVisionVolume(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
	// Set reasonable default values.
	SizeInTiles = 256;
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

void ARTSVisionVolume::BeginPlay()
{
    Super::BeginPlay();

	// Get vision world size.
	UBrushComponent* VisionBrushComponent = GetBrushComponent();
	FBoxSphereBounds VisionBounds = VisionBrushComponent->CalcBounds(VisionBrushComponent->GetComponentTransform());

	SizeInWorld = VisionBounds.BoxExtent * 2;

	// Calculate tile size.
    TileSize = SizeInWorld.X / SizeInTiles;
}
