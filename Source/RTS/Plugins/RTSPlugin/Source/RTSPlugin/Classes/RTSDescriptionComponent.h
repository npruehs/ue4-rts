#pragma once

#include "RTSPluginPCH.h"

#include "Components/ActorComponent.h"

#include "RTSDescriptionComponent.generated.h"


/**
* Adds a localizable description to the actor.
*/
UCLASS(meta = (BlueprintSpawnableComponent))
class RTSPLUGIN_API URTSDescriptionComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    /** Description of the actor. */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS")
    FText Description;
};
