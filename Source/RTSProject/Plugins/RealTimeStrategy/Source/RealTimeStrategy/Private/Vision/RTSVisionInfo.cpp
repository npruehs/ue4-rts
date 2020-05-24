#include "Vision/RTSVisionInfo.h"

#include "EngineUtils.h"
#include "Net/UnrealNetwork.h"

#include "RTSLog.h"
#include "RTSOwnerComponent.h"
#include "RTSPlayerController.h"
#include "RTSPlayerState.h"
#include "RTSTeamInfo.h"
#include "Vision/RTSVisionComponent.h"
#include "Vision/RTSVisionVolume.h"


ARTSVisionInfo::ARTSVisionInfo(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
    : Super(ObjectInitializer)
{
	// Enable replication.
	SetReplicates(true);
	bAlwaysRelevant = true;
	NetUpdateFrequency = 1.0f;

	// Force ReceivedTeamIndex() on clients.
	TeamIndex = 255;

	PrimaryActorTick.bCanEverTick = true;
}

void ARTSVisionInfo::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME_CONDITION(ARTSVisionInfo, TeamIndex, COND_InitialOnly);
}

void ARTSVisionInfo::Initialize(ARTSVisionVolume* InVisionVolume)
{
    VisionVolume = InVisionVolume;

    if (!VisionVolume)
    {
        UE_LOG(LogRTS, Warning, TEXT("No vision volume found, won't update vision."));
        return;
    }

    int32 SizeInTiles = VisionVolume->GetSizeInTiles();
    Tiles.SetNumZeroed(SizeInTiles * SizeInTiles);

    for (int32 Index = 0; Index < Tiles.Num(); ++Index)
    {
        Tiles[Index] = VisionVolume->GetMinimumVisionState();
    }

    UE_LOG(LogRTS, Log, TEXT("Set up %s with %s."), *GetName(), *VisionVolume->GetName());
}

void ARTSVisionInfo::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!VisionVolume)
	{
		return;
	}

	// Reset tiles.
	int32 SizeInTiles = VisionVolume->GetSizeInTiles();
	Tiles.SetNumZeroed(SizeInTiles * SizeInTiles);

    const ERTSVisionState VisionAfterVisible =
        FMath::Max(ERTSVisionState::VISION_Known, VisionVolume->GetMinimumVisionState());

	for (int32 Index = 0; Index < Tiles.Num(); ++Index)
	{
		if (Tiles[Index] == ERTSVisionState::VISION_Visible)
		{
			Tiles[Index] = VisionAfterVisible;
		}
	}

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

		if (!PlayerOwner->GetTeam())
		{
			continue;
		}

		if (PlayerOwner->GetTeam()->GetTeamIndex() != TeamIndex)
		{
			continue;
		}

		// Convert location and sight radius to tile space.
		FVector ActorLocationWorld = Actor->GetActorLocation();
		FIntVector ActorLocationTile = VisionVolume->WorldToTile(ActorLocationWorld);
		int32 ActorSightRadiusTile = FMath::FloorToInt(VisionComponent->GetSightRadius() / VisionVolume->GetTileSize());

		/*UE_LOG(LogRTS, Log, TEXT("ActorLocationWorld: %s"), *ActorLocationWorld.ToString());
		UE_LOG(LogRTS, Log, TEXT("ActorLocationTile: %s"), *ActorLocationTile.ToString());
		UE_LOG(LogRTS, Log, TEXT("VisionComponent->SightRadius: %f"), VisionComponent->SightRadius);
		UE_LOG(LogRTS, Log, TEXT("VisionVolume->SizePerTile: %f"), VisionVolume->SizePerTile);
		UE_LOG(LogRTS, Log, TEXT("ActorSightRadiusTile: %i"), ActorSightRadiusTile);*/

		// XXX VERY simple circle algorithm.
		for (int32 RadiusY = -ActorSightRadiusTile; RadiusY <= ActorSightRadiusTile; RadiusY++)
		{
			for (int32 RadiusX = -ActorSightRadiusTile; RadiusX <= ActorSightRadiusTile; RadiusX++)
			{
				int32 TileX = ActorLocationTile.X + RadiusX;
				int32 TileY = ActorLocationTile.Y + RadiusY;

				// Check if within circle.
				if (TileX >= 0 &&
					TileY >= 0 &&
					TileX < SizeInTiles &&
					TileY < SizeInTiles &&
					(RadiusX * RadiusX + RadiusY * RadiusY < ActorSightRadiusTile * ActorSightRadiusTile))
				{
					int32 TileIndex = GetTileIndex(TileX, TileY);
					Tiles[TileIndex] = ERTSVisionState::VISION_Visible;

					//UE_LOG(LogRTS, Log, TEXT("Revealed tile (%i, %i)."), TileX, TileY);
				}
			}
		}
	}
}

uint8 ARTSVisionInfo::GetTeamIndex() const
{
    return TeamIndex;
}

void ARTSVisionInfo::SetTeamIndex(uint8 NewTeamIndex)
{
	TeamIndex = NewTeamIndex;
	NotifyPlayerVisionInfoAvailable();
}

ERTSVisionState ARTSVisionInfo::GetVision(int32 X, int32 Y) const
{
	int32 TileIndex = GetTileIndex(X, Y);
	return Tiles[TileIndex];
}

ARTSVisionInfo* ARTSVisionInfo::GetVisionInfoForTeam(UObject* WorldContextObject, uint8 InTeamIndex)
{
	UWorld* World = WorldContextObject->GetWorld();

	for (TActorIterator<ARTSVisionInfo> It(World); It; ++It)
	{
		ARTSVisionInfo* VisionInfo = *It;

		if (VisionInfo->TeamIndex == InTeamIndex)
		{
			return VisionInfo;
		}
	}

	return nullptr;
}

bool ARTSVisionInfo::GetTileCoordinates(int Index, int* OutX, int* OutY) const
{
	if (Index < 0 || Index >= Tiles.Num())
	{
		return false;
	}

	int32 SizeInTiles = VisionVolume->GetSizeInTiles();

	*OutX = Index % SizeInTiles;
	*OutY = Index / SizeInTiles;
	return true;
}

int32 ARTSVisionInfo::GetTileIndex(int X, int Y) const
{
	return Y * VisionVolume->GetSizeInTiles() + X;
}

void ARTSVisionInfo::NotifyPlayerVisionInfoAvailable()
{
	// Notify local player.
	UWorld* World = GetWorld();

	if (!World)
	{
		return;
	}

	ARTSPlayerController* Player = Cast<ARTSPlayerController>(World->GetFirstPlayerController());

	if (!Player)
	{
		return;
	}

	ARTSTeamInfo* Team = Player->GetTeamInfo();

	if (!Team || Team->GetTeamIndex() != TeamIndex)
	{
		return;
	}

	Player->NotifyOnVisionInfoAvailable(this);
}

void ARTSVisionInfo::ReceivedTeamIndex()
{
	NotifyPlayerVisionInfoAvailable();
}
