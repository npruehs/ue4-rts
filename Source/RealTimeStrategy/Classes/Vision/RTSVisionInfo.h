#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Info.h"

#include "Vision/RTSVisionState.h"
#include "Vision/RTSVisionTile.h"

#include "RTSVisionInfo.generated.h"


class UWorld;

class ARTSVisionVolume;


/**
* Defines the visible areas for a player or team.
*/
UCLASS(Category="RTS")
class REALTIMESTRATEGY_API ARTSVisionInfo : public AInfo
{
	GENERATED_BODY()

public:
	ARTSVisionInfo(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Prepares this actor for gameplay. */
	void Initialize(ARTSVisionVolume* InVisionVolume);

	/** Whether vision info is already set up and available. */
	bool IsInitialized() const;

	/** Whether the whole map is currently being considered revealed (e.g. cheat, cinematic mode.) */
	bool IsRevealed() const;

	/** Sets whether the whole map is currently being considered revealed (e.g. cheat, cinematic mode.) */
	void SetRevealed(bool bInRevealed);

	/** Gets the index of the team this actor keeps track of the vision for. */
	uint8 GetTeamIndex() const;

	/** Sets the index of the team this actor keeps track of the vision for. */
	void SetTeamIndex(uint8 NewTeamIndex);

	/** Gets the state of the tile with the specified coordinates. */
	ERTSVisionState GetVision(int32 X, int32 Y) const;

	/** Gets vision information for the specified team. */
	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContextObject"))
	static ARTSVisionInfo* GetVisionInfoForTeam(UObject* WorldContextObject, uint8 InTeamIndex);

	/** Applies vision for the passed actor for this team. */
	void ApplyVisionForActor(AActor* Actor, TArray<int32>& CachedTiles);

	/** Resets all vision applied by the passed actor for this team. */
	void ResetVisionForActor(AActor* Actor);

private:
	/** Whether vision info is already set up and available. */
	bool bInitialized;

	/** Whether the whole map is currently being considered revealed (e.g. cheat, cinematic mode.) */
	bool bRevealed;

	/** Index of the team this actor keeps track of the vision for. */
	UPROPERTY(ReplicatedUsing = ReceivedTeamIndex)
	uint8 TeamIndex;

	UPROPERTY()
	ARTSVisionVolume* VisionVolume;

	/** Which tiles are currently known. */
	TArray<bool> KnownTiles;

	/** Which tiles are currently visible. */
	UPROPERTY()
	TArray<FRTSVisionTile> VisibleTiles;

	/** Temporary grid for line of sight checks. */
	TArray<int32> LocalVisionGrid;

	bool GetTileCoordinates(int Index, int* OutX, int* OutY) const;
	int32 GetTileIndex(int X, int Y) const;

	void NotifyPlayerVisionInfoAvailable();

	UFUNCTION()
	virtual void ReceivedTeamIndex();

	/** Checks whether we've got vision on the specified tile; cached for a short duration. */
	bool HasLocalVisionAt(int32 LocalTileX, int32 LocalTileY, int32 ActorSightRadiusTile, int32 GlobalTileX,
	                      int32 GlobalTileY, int32 ActorLocationTileX, int32 ActorLocationTileY,
	                      int32 ActorLocationHeightLevel);
};
