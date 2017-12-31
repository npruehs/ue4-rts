#pragma once

#include "RTSPluginPCH.h"

#include "Components/ActorComponent.h"

#include "RTSPlayerAdvantageComponent.generated.h"


/**
* Attach to player or AI controllers. Provides bonuses for various gameplay elements.
*/
UCLASS(meta = (BlueprintSpawnableComponent))
class RTSPLUGIN_API URTSPlayerAdvantageComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    URTSPlayerAdvantageComponent(const FObjectInitializer& ObjectInitializer);

    /** Whether units controlled by the player are invulnerable, or not. */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS|Advantage")
    bool bGodModeEnabled;

    /** Factor to multiply all construction and production speed with. */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS|Advantage")
    float SpeedBoostFactor;
};
