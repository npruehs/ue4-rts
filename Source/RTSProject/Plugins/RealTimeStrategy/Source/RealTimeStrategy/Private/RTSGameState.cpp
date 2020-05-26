#include "RTSGameState.h"

#include "EngineUtils.h"
#include "GameFramework/Actor.h"

#include "Vision/RTSFogOfWarActor.h"
#include "Vision/RTSVisionInfo.h"
#include "Vision/RTSVisionVolume.h"

void ARTSGameState::HandleBeginPlay()
{
    // Make sure all actors have begun play.
    Super::HandleBeginPlay();

    // Setup vision.
    ARTSFogOfWarActor* FogOfWarActor = nullptr;
    TArray<ARTSVisionInfo*> VisionInfos;
    ARTSVisionVolume* VisionVolume = nullptr;

    for (TActorIterator<AActor> It(GetWorld()); It; ++It)
    {
        AActor* Actor = *It;

        if (IsValid(Actor))
        {
            if (Actor->IsA(ARTSFogOfWarActor::StaticClass()))
            {
                FogOfWarActor = Cast<ARTSFogOfWarActor>(Actor);
            }
            else if (Actor->IsA(ARTSVisionInfo::StaticClass()))
            {
                ARTSVisionInfo* VisionInfo = Cast<ARTSVisionInfo>(Actor);
                VisionInfos.Add(VisionInfo);
            }
            else if (Actor->IsA(ARTSVisionVolume::StaticClass()))
            {
                VisionVolume = Cast<ARTSVisionVolume>(Actor);
            }
        }
    }

    if (IsValid(VisionVolume))
    {
        VisionVolume->Initialize();
    }

    for (ARTSVisionInfo* VisionInfo : VisionInfos)
    {
        VisionInfo->Initialize(VisionVolume);
    }

    if (IsValid(FogOfWarActor))
    {
        FogOfWarActor->Initialize(VisionVolume);
    }
}
