#include "Vision/RTSVisionInfo.h"

#include "EngineUtils.h"
#include "Net/UnrealNetwork.h"

#include "RTSLog.h"
#include "RTSOwnerComponent.h"
#include "RTSPlayerController.h"
#include "RTSPlayerState.h"
#include "RTSTeamInfo.h"
#include "Vision/RTSVisionComponent.h"
#include "Vision/RTSVisionInfoTileList.h"
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

    bInitialized = false;
    bRevealed = false;
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

    KnownTiles.SetNum(SizeInTiles * SizeInTiles);
    VisibleTiles.SetNum(SizeInTiles * SizeInTiles);

    bInitialized = true;

    UE_LOG(LogRTS, Log, TEXT("Set up %s with %s."), *GetName(), *VisionVolume->GetName());

    // Players might already have their team info, before we had been initialized. Give them another chance now.
    NotifyPlayerVisionInfoAvailable();
}

bool ARTSVisionInfo::IsInitialized() const
{
    return bInitialized;
}

bool ARTSVisionInfo::IsRevealed() const
{
    return bRevealed;
}

void ARTSVisionInfo::SetRevealed(bool bInRevealed)
{
    bRevealed = bInRevealed;
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
    if (!VisionVolume)
    {
        return ERTSVisionState::VISION_Unknown;
    }

    if (bRevealed)
    {
        return ERTSVisionState::VISION_Visible;
    }

	int32 TileIndex = GetTileIndex(X, Y);
	
    if (VisibleTiles.IsValidIndex(TileIndex) && VisibleTiles[TileIndex].IsVisible())
    {
        return FMath::Max(VisionVolume->GetMinimumVisionState(), ERTSVisionState::VISION_Visible);
    }
    else if (KnownTiles.IsValidIndex(TileIndex) && KnownTiles[TileIndex])
    {
        return FMath::Max(VisionVolume->GetMinimumVisionState(), ERTSVisionState::VISION_Known);
    }
    else
    {
        return VisionVolume->GetMinimumVisionState();
    }
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

void ARTSVisionInfo::ApplyVisionForActor(AActor* Actor, TArray<int32>& CachedTiles)
{
    if (!VisionVolume)
    {
        return;
    }

    // Verify team.
    URTSOwnerComponent* OwnerComponent;
    ARTSPlayerState* PlayerOwner;
    URTSVisionComponent* VisionComponent;

    OwnerComponent = Actor->FindComponentByClass<URTSOwnerComponent>();

    if (!OwnerComponent)
    {
        return;
    }

    PlayerOwner = OwnerComponent->GetPlayerOwner();

    if (!PlayerOwner)
    {
        return;
    }

    if (!PlayerOwner->GetTeam())
    {
        return;
    }

    VisionComponent = Actor->FindComponentByClass<URTSVisionComponent>();

    if (!VisionComponent)
    {
        return;
    }

    if (PlayerOwner->GetTeam()->GetTeamIndex() != TeamIndex)
    {
        return;
    }

    if (CachedTiles.Num() == 0)
    {
        // Convert location and sight radius to tile space.
        FVector ActorLocationWorld;
        FIntVector ActorLocationTile;
        int32 ActorLocationHeightLevel;
        int32 ActorSightRadiusTile;
        int32 ActorSightRadiusTileSqr;
        int32 WorldTileSize;

        ActorLocationWorld = Actor->GetActorLocation();
        ActorLocationTile = VisionComponent->ActorLocationTile;
        ActorLocationHeightLevel = VisionVolume->GetTileHeight(ActorLocationTile);
        ActorSightRadiusTile = FMath::FloorToInt(VisionComponent->GetSightRadius() / VisionVolume->GetTileSize());
        ActorSightRadiusTileSqr = ActorSightRadiusTile * ActorSightRadiusTile;
        WorldTileSize = VisionVolume->GetSizeInTiles();

        LocalVisionGrid.Init(0, (2 * ActorSightRadiusTile + 1) * (2 * ActorSightRadiusTile + 1));

        // XXX VERY simple circle algorithm.
        int32 TileX;
        int32 TileY;

        int32 RadiusXSqr;
        int32 RadiusYSqr;

        int32 LocalTileX;
        int32 LocalTileY;

        for (int32 RadiusY = -ActorSightRadiusTile; RadiusY <= ActorSightRadiusTile; RadiusY++)
        {
            TileY = ActorLocationTile.Y + RadiusY;
            RadiusYSqr = RadiusY * RadiusY;
            LocalTileY = RadiusY + ActorSightRadiusTile;

            if (TileY >= 0 && TileY < WorldTileSize)
            {
                for (int32 RadiusX = -ActorSightRadiusTile; RadiusX <= ActorSightRadiusTile; RadiusX++)
                {
                    TileX = ActorLocationTile.X + RadiusX;
                    RadiusXSqr = RadiusX * RadiusX;
                    LocalTileX = RadiusX + ActorSightRadiusTile;

                    // Check if within circle.
                    if (TileX >= 0 && TileX < WorldTileSize && (RadiusXSqr + RadiusYSqr <= ActorSightRadiusTileSqr))
                    {
                        if (VisionComponent->IgnoresHeightLevels() ||
                            HasLocalVisionAt(LocalTileX, LocalTileY, ActorSightRadiusTile, TileX, TileY,
                                ActorLocationTile.X, ActorLocationTile.Y, ActorLocationHeightLevel))
                        {
                            CachedTiles.Add(GetTileIndex(TileX, TileY));
                        }
                    }
                }
            }
        }
    }

    for (int32 TileIndex : CachedTiles)
    {
        VisibleTiles[TileIndex].AddActor(Actor);
    }

    FRTSVisionInfoTileList VisionInfoTileList;
    VisionInfoTileList.VisionInfo = this;
    VisionInfoTileList.VisibleTiles = CachedTiles;

    VisionComponent->VisibleTiles.Add(VisionInfoTileList);
}

void ARTSVisionInfo::ResetVisionForActor(AActor* Actor)
{
    // Verify vision.
    URTSVisionComponent* VisionComponent = Actor->FindComponentByClass<URTSVisionComponent>();

    if (!VisionComponent)
    {
        return;
    }

    // Reset vision.
    for (FRTSVisionInfoTileList& VisionInfoTileList : VisionComponent->VisibleTiles)
    {
        if (VisionInfoTileList.VisionInfo != this)
        {
            continue;
        }

        for (int32 Index = 0; Index < VisionInfoTileList.VisibleTiles.Num(); ++Index)
        {
            VisibleTiles[VisionInfoTileList.VisibleTiles[Index]].RemoveActor(Actor);
            KnownTiles[VisionInfoTileList.VisibleTiles[Index]] = true;
        }
    }
}

bool ARTSVisionInfo::GetTileCoordinates(int Index, int* OutX, int* OutY) const
{
	if (Index < 0 || Index >= VisibleTiles.Num())
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

bool ARTSVisionInfo::HasLocalVisionAt(int32 LocalTileX, int32 LocalTileY, int32 ActorSightRadiusTile,
    int32 GlobalTileX, int32 GlobalTileY, int32 ActorLocationTileX, int32 ActorLocationTileY,
    int32 ActorLocationHeightLevel)
{
    const int32 VISION_NOTCHECKED = 0;
    const int32 VISION_NO = 1;
    const int32 VISION_YES = 2;

    int32 LocalTileIndex;

    // Always got vision on our own location.
    if (GlobalTileX == ActorLocationTileX && GlobalTileY == ActorLocationTileY)
    {
        return true;
    }

    // Have we calculated and cached this already?
    LocalTileIndex = LocalTileY * (2 * ActorSightRadiusTile + 1) + LocalTileX;
    int32 CheckedVision = LocalVisionGrid[LocalTileIndex];

    if (CheckedVision != VISION_NOTCHECKED)
    {
        return CheckedVision == VISION_YES;
    }

    // Check height level.
    int32 TileHeightLevel = VisionVolume->GetTileHeight(FIntVector(GlobalTileX, GlobalTileY, 0));

    if (TileHeightLevel > ActorLocationHeightLevel)
    {
        // Above actor.
        LocalVisionGrid[LocalTileIndex] = VISION_NO;
        return false;
    }

    int32 x0;
    int32 y0;

    int32 x0_;
    int32 y0_;

    // Check line of sight.
    // Single Bresenham step.
    x0 = GlobalTileX;
    y0 = GlobalTileY;

    x0_ = LocalTileX;
    y0_ = LocalTileY;

    int32 x1 = ActorLocationTileX;
    int32 y1 = ActorLocationTileY;

    int32 dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int32 dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int32 err = dx + dy, e2; /* error value e_xy */

    e2 = 2 * err;
    if (e2 > dy)
    {
        err += dy;
        x0 += sx;
        x0_ += sx;
    } /* e_xy+e_x > 0 */
    if (e2 < dx)
    {
        err += dx;
        y0 += sy;
        y0_ += sy;
    } /* e_xy+e_y < 0 */

    // Recursive call: We've got vision here, check if we had vision one tile closer to the actor.
    bool bHasVisionThere = HasLocalVisionAt(x0_, y0_, ActorSightRadiusTile, x0, y0, ActorLocationTileX,
        ActorLocationTileY, ActorLocationHeightLevel);
    LocalVisionGrid[LocalTileIndex] = bHasVisionThere ? VISION_YES : VISION_NO;
    return bHasVisionThere;
}
