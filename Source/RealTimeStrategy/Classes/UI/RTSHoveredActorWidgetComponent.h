#pragma once

#include "CoreMinimal.h"

#include "UI/RTSActorWidgetComponent.h"

#include "RTSHoveredActorWidgetComponent.generated.h"


/**
* Adds a widget for showing hovered actor data to the actor.
*/
UCLASS(Blueprintable)
class REALTIMESTRATEGY_API URTSHoveredActorWidgetComponent : public URTSActorWidgetComponent
{
    GENERATED_BODY()

public:
    /** Event when the actor was hovered. */
    UFUNCTION(BlueprintImplementableEvent, Category = "RTS")
    void UpdateData(AActor* Actor);
};
