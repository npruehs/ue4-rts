#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"

#include "RTSNameComponent.generated.h"


/**
* Adds a localizable name to the actor.
*/
UCLASS(meta = (BlueprintSpawnableComponent))
class REALTIMESTRATEGY_API URTSNameComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    /** Gets the name of the actor. */
    UFUNCTION(BlueprintPure)
    FText GetNameRTS() const;

    UFUNCTION(BlueprintCallable)
    void SetName(const FText& NewName);

private:
    /** Name of the actor. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS")
    FText Name;
};
