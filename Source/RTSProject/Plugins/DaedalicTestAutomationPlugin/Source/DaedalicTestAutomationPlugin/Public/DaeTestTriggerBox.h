#pragma once

#include <CoreMinimal.h>
#include <Engine/TriggerBox.h>
#include "DaeTestTriggerBox.generated.h"

/** Trigger box to be used in automated tests. */
UCLASS()
class DAEDALICTESTAUTOMATIONPLUGIN_API ADaeTestTriggerBox : public ATriggerBox
{
    GENERATED_BODY()

public:
    virtual void BeginPlay() override;

    /** Whether this trigger box has been triggered at least once. */
    UFUNCTION(BlueprintPure)
    bool WasTriggered() const;

private:
    /** Whether this trigger box has been triggered at least once. */
    bool bWasTriggered;

    UFUNCTION()
    void OnActorBeginOverlapBroadcast(AActor* OverlappedActor, AActor* OtherActor);
};
