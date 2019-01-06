#pragma once

#include "RealTimeStrategyPCH.h"

#include "GameFramework/Volume.h"

#include "RTSCameraBoundsVolume.generated.h"


/**
 * Volume that restricts RTS camera movement.
 * RTS cameras are not allowed to move outside of this volume.
 * There should never be more than one camera bounds volume per level.
 */
UCLASS()
class REALTIMESTRATEGY_API ARTSCameraBoundsVolume : public AVolume
{
    GENERATED_UCLASS_BODY()
};
