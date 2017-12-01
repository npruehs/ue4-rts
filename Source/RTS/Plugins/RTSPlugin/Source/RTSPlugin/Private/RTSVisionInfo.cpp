#include "RTSPluginPrivatePCH.h"
#include "RTSVisionInfo.h"

#include "EngineUtils.h"

#include "RTSOwnerComponent.h"
#include "RTSPlayerState.h"
#include "RTSVisionComponent.h"
#include "RTSVisionVolume.h"


ARTSVisionInfo::ARTSVisionInfo()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ARTSVisionInfo::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!VisionVolume)
	{
		return;
	}

	// Reset tiles.
	FIntVector TileSize = VisionVolume->GetTileSize();
	Tiles.SetNumZeroed(TileSize.X * TileSize.Y);

	for (int32 Index = 0; Index < Tiles.Num(); ++Index)
	{
		if (Tiles[Index] == ERTSVisionState::VISION_Visible)
		{
			Tiles[Index] = ERTSVisionState::VISION_Known;
		}
	}

	FIntVector WorldTileSize = VisionVolume->GetTileSize();

	// Apply vision.
	for (TActorIterator<AActor> ActorIt(GetWorld()); ActorIt; ++ActorIt)
	{
		AActor* Actor = *ActorIt;

		// Verify vision.
		URTSVisionComponent* VisionComponent = Actor->FindComponentByClass<URTSVisionComponent>();

		if (!VisionComponent)
		{
			continue;
		}

		// Verify team.
		URTSOwnerComponent* OwnerComponent = Actor->FindComponentByClass<URTSOwnerComponent>();
		ARTSPlayerState* PlayerOwner = OwnerComponent->GetPlayerOwner();

		if (!PlayerOwner)
		{
			continue;
		}

		if (!PlayerOwner->Team)
		{
			continue;
		}

		if (PlayerOwner->Team->TeamIndex != TeamIndex)
		{
			continue;
		}

		// Convert location and sight radius to tile space.
		FVector ActorLocationWorld = Actor->GetActorLocation();
		FIntVector ActorLocationTile = VisionVolume->WorldToTile(ActorLocationWorld);
		int32 ActorSightRadiusTile = FMath::FloorToInt(VisionComponent->SightRadius / VisionVolume->SizePerTile);

		/*UE_LOG(RTSLog, Log, TEXT("ActorLocationWorld: %s"), *ActorLocationWorld.ToString());
		UE_LOG(RTSLog, Log, TEXT("ActorLocationTile: %s"), *ActorLocationTile.ToString());
		UE_LOG(RTSLog, Log, TEXT("VisionComponent->SightRadius: %f"), VisionComponent->SightRadius);
		UE_LOG(RTSLog, Log, TEXT("VisionVolume->SizePerTile: %f"), VisionVolume->SizePerTile);
		UE_LOG(RTSLog, Log, TEXT("ActorSightRadiusTile: %i"), ActorSightRadiusTile);*/

		// XXX VERY simple circle algorithm.
		for (int32 RadiusY = -ActorSightRadiusTile; RadiusY < ActorSightRadiusTile; RadiusY++)
		{
			for (int32 RadiusX = -ActorSightRadiusTile; RadiusX < ActorSightRadiusTile; RadiusX++)
			{
				int32 TileX = ActorLocationTile.X + RadiusX;
				int32 TileY = ActorLocationTile.Y + RadiusY;

				// Check if within circle.
				if (TileX >= 0 &&
					TileY >= 0 &&
					TileX < WorldTileSize.X &&
					TileY < WorldTileSize.Y &&
					(RadiusX * RadiusX + RadiusY * RadiusY < ActorSightRadiusTile * ActorSightRadiusTile))
				{
					int32 TileIndex = GetTileIndex(TileX, TileY);
					Tiles[TileIndex] = ERTSVisionState::VISION_Visible;

					//UE_LOG(RTSLog, Log, TEXT("Revealed tile (%i, %i)."), TileX, TileY);
				}
			}
		}
	}
}

ERTSVisionState ARTSVisionInfo::GetVision(int32 X, int32 Y) const
{
	int32 TileIndex = GetTileIndex(X, Y);
	return Tiles[TileIndex];
}

ARTSVisionInfo* ARTSVisionInfo::GetLocalVisionInfo(UWorld* World)
{
	APlayerController* Player = World->GetFirstPlayerController();

	if (Player)
	{
		ARTSPlayerState* PlayerState = Cast<ARTSPlayerState>(Player->PlayerState);

		if (PlayerState)
		{
			if (PlayerState->Team)
			{
				for (TActorIterator<ARTSVisionInfo> It(World); It; ++It)
				{
					if (PlayerState->Team->TeamIndex == (*It)->TeamIndex)
					{
						return *It;
					}
				}

				UE_LOG(RTSLog, Warning, TEXT("No vision info found for team %i."), PlayerState->Team->TeamIndex);
			}
			else
			{
				UE_LOG(RTSLog, Warning, TEXT("Local player has no team, unable to get vision info."));
			}
		}
		else
		{
			UE_LOG(RTSLog, Warning, TEXT("Local player has no ARTSPlayerState, unable to get vision info."));
		}
	}
	else
	{
		UE_LOG(RTSLog, Warning, TEXT("No local player, unable to get vision info."));
	}

	return nullptr;
}

void ARTSVisionInfo::BeginPlay()
{
	for (TActorIterator<ARTSVisionVolume> It(GetWorld()); It; ++It)
	{
		VisionVolume = *It;
		break;
	}

	if (!VisionVolume)
	{
		UE_LOG(RTSLog, Warning, TEXT("No vision volume found, won't update vision."));
	}
}

bool ARTSVisionInfo::GetTileCoordinates(int Index, int* OutX, int* OutY) const
{
	if (Index < 0 || Index >= Tiles.Num())
	{
		return false;
	}

	FIntVector TileSize = VisionVolume->GetTileSize();

	*OutX = Index % TileSize.X;
	*OutY = Index / TileSize.X;
	return true;
}

int32 ARTSVisionInfo::GetTileIndex(int X, int Y) const
{
	return Y * VisionVolume->GetTileSize().X + X;
}
