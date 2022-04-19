#pragma once

#include "CoreMinimal.h"

#include "RTSVisionInfoTileList.generated.h"

class ARTSVisionInfo;

/** Tiles associated with a specific vision info. */
USTRUCT()
struct REALTIMESTRATEGY_API FRTSVisionInfoTileList
{
    GENERATED_BODY()

public:
    UPROPERTY()
    ARTSVisionInfo* VisionInfo;

    TArray<int32> VisibleTiles;
};
