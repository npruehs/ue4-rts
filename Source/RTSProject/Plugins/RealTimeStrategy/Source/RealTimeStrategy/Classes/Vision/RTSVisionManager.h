#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "Vision/RTSVisibleActor.h"
#include "Vision/RTSVisionActor.h"

#include "RTSVisionManager.generated.h"


class ARTSFogOfWarActor;
class ARTSPlayerState;
class ARTSVisionInfo;
class ARTSVisionVolume;


/** Updates actor vision and visibility. */
UCLASS()
class REALTIMESTRATEGY_API ARTSVisionManager : public AActor
{
    GENERATED_BODY()

public:
    ARTSVisionManager(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    void Initialize();

    virtual void Tick(float DeltaSeconds) override;

    /** Sets the player state to use for the local player. */
    void SetLocalPlayerState(ARTSPlayerState* InLocalPlayerState);

    /** Sets the vision info to use for the local player. */
    void SetLocalVisionInfo(ARTSVisionInfo* InLocalVisionInfo);

    /** Registers the specified actor for updating its own visibility. */
    void AddVisibleActor(AActor* Actor);

    /** Unregisters the specified actor for updating its own visibility. */
    void RemoveVisibleActor(AActor* Actor);

    /** Registers the specified actor for updating team vision. */
    void AddVisionActor(AActor* Actor);

    /** Unregisters the specified actor for updating team vision. */
    void RemoveVisionActor(AActor* Actor);

private:
    /** Fog of war actor of the current world. */
    UPROPERTY()
    ARTSFogOfWarActor* FogOfWarActor;

    /** Vision info for all teams. */
    UPROPERTY()
    TArray<ARTSVisionInfo*> VisionInfos;

    /** Player State for the local player. */
    UPROPERTY()
    ARTSPlayerState* LocalPlayerState;

    /** Vision info for the local player. */
    UPROPERTY()
    ARTSVisionInfo* LocalVisionInfo;

    /** Vision volume of the current world. */
    UPROPERTY()
    ARTSVisionVolume* VisionVolume;

    /** Actors that may become invisible. */
    UPROPERTY()
    TArray<FRTSVisibleActor> VisibleActors;

    /** All actors that may update team vision. */
    UPROPERTY()
    TArray<FRTSVisionActor> VisionActors;

    /** Checks whether vision needs to be updated for the specified actor, and does so if necessary. */
    void UpdateVisionActor(const FRTSVisionActor& VisionActor);

    /** Resets vision for the specified actor (e.g. because update is imminent, or actor has been removed.) */
    void ResetVisionForActor(const FRTSVisionActor& VisionActor);
};
