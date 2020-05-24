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
    ARTSVisionInfo* VisionInfo = nullptr;
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
            else if (Actor->IsA(ARTSVisionVolume::StaticClass()))
            {
                VisionVolume = Cast<ARTSVisionVolume>(Actor);
            }
            else if (Actor->IsA(ARTSVisionInfo::StaticClass()))
            {
                VisionInfo = Cast<ARTSVisionInfo>(Actor);
            }
        }
    }

    if (IsValid(VisionVolume))
    {
        VisionVolume->Initialize();
    }

    if (IsValid(VisionInfo))
    {
        VisionInfo->Initialize(VisionVolume);
    }

    if (IsValid(FogOfWarActor))
    {
        FogOfWarActor->Initialize(VisionVolume);
    }
}
