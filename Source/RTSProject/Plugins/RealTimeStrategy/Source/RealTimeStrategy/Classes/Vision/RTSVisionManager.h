#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "Vision/RTSVisibleActor.h"

#include "RTSVisionManager.generated.h"


class ARTSFogOfWarActor;
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

    /** Sets the vision info to use for the local player. */
    void SetLocalVisionInfo(ARTSVisionInfo* InLocalVisionInfo);

    /** Registers the specified actor for updating its own visibility. */
    void AddVisibleActor(AActor* Actor);

    /** Unregisters the specified actor for updating its own visibility. */
    void RemoveVisibleActor(AActor* Actor);

private:
    /** Fog of war actor of the current world. */
    UPROPERTY()
    ARTSFogOfWarActor* FogOfWarActor;

    /** Vision info for all teams. */
    UPROPERTY()
    TArray<ARTSVisionInfo*> VisionInfos;

    /** Vision info for the local player. */
    UPROPERTY()
    ARTSVisionInfo* LocalVisionInfo;

    /** Vision volume of the current world. */
    UPROPERTY()
    ARTSVisionVolume* VisionVolume;

    /** Actors that may become invisible. */
    UPROPERTY()
    TArray<FRTSVisibleActor> VisibleActors;
};
