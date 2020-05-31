#pragma once

#include <CoreMinimal.h>
#include <Kismet/BlueprintFunctionLibrary.h>
#include "DaeTestAssumeBlueprintFunctionLibrary.generated.h"

/** Utility functions for assuming state before acting in automated tests. */
UCLASS()
class DAEDALICTESTAUTOMATIONPLUGIN_API UDaeTestAssumeBlueprintFunctionLibrary
    : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /** Expects the specified value to be true. Failed assumptions will cause automated tests to be skipped instead of failed. */
    UFUNCTION(BlueprintCallable, meta = (HidePin = "Context", DefaultToSelf = "Context"))
    static void AssumeTrue(bool bValue, const FString& What, UObject* Context = nullptr);

    /** Expects the specified value to be true. Failed assumptions will cause automated tests to be skipped instead of failed. */
    UFUNCTION(BlueprintCallable, meta = (HidePin = "Context", DefaultToSelf = "Context"))
    static void AssumeFalse(bool bValue, const FString& What, UObject* Context = nullptr);

private:
    static void OnTestSkipped(UObject* Context, const FString& Message);
};
