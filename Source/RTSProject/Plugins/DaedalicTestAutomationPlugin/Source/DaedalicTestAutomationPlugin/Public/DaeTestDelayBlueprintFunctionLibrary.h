#pragma once

#include <CoreMinimal.h>
#include <Engine/LatentActionManager.h>
#include <Kismet/BlueprintFunctionLibrary.h>
#include "DaeTestDelayBlueprintFunctionLibrary.generated.h"

class ADaeTestTriggerBox;

/** Utility functions for sequencing actions in automated tests. */
UCLASS()
class DAEDALICTESTAUTOMATIONPLUGIN_API UDaeTestDelayBlueprintFunctionLibrary
    : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /** Triggers the output link after the specified number of frames. */
    UFUNCTION(BlueprintCallable,
              meta = (Latent, WorldContext = "WorldContextObject", LatentInfo = "LatentInfo"))
    static void DelayFrames(UObject* WorldContextObject, struct FLatentActionInfo LatentInfo,
                            int32 NumFrames = 1);

    /** Triggers the output link after the specified trigger box has been triggered. */
    UFUNCTION(BlueprintCallable,
              meta = (Latent, WorldContext = "WorldContextObject", LatentInfo = "LatentInfo"))
    static void DelayUntilTriggered(UObject* WorldContextObject,
                                    struct FLatentActionInfo LatentInfo,
                                    ADaeTestTriggerBox* TestTriggerBox);
};
