#pragma once

#include <CoreMinimal.h>
#include <Engine/EngineBaseTypes.h>
#include <Kismet/BlueprintFunctionLibrary.h>
#include "DaeTestInputBlueprintFunctionLibrary.generated.h"

/** Utility functions for simulating input in automated tests. */
UCLASS()
class DAEDALICTESTAUTOMATIONPLUGIN_API UDaeTestInputBlueprintFunctionLibrary
    : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /** Applies the input action with the specified name once. */
    UFUNCTION(BlueprintCallable, meta = (HidePin = "Context", DefaultToSelf = "Context"))
    static void ApplyInputAction(UObject* Context, const FName& ActionName,
                                 EInputEvent InputEventType = EInputEvent::IE_Pressed);

    /** Applies the input axis with the specified name. Pass AxisValue 0.0f to reset the input axis. */
    UFUNCTION(BlueprintCallable, meta = (HidePin = "Context", DefaultToSelf = "Context"))
    static void ApplyInputAxis(UObject* Context, const FName& AxisName, float AxisValue = 1.0f);
};
