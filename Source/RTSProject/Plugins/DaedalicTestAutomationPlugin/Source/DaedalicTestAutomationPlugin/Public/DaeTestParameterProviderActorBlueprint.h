#pragma once

#include <CoreMinimal.h>
#include <Engine/Blueprint.h>
#include "DaeTestParameterProviderActorBlueprint.generated.h"

/** Provides a set of parameters for tests, which are run once per parameter. */
UCLASS(BlueprintType)
class DAEDALICTESTAUTOMATIONPLUGIN_API UDaeTestParameterProviderActorBlueprint : public UBlueprint
{
    GENERATED_BODY()

public:
#if WITH_EDITOR
    // UBlueprint interface
    virtual bool SupportedByDefaultBlueprintFactory() const override
    {
        return false;
    }
    // End of UBlueprint interface
#endif
};
