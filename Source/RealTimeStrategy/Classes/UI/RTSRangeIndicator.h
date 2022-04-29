#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RTSRangeIndicator.generated.h"

/** Previews the range of an action.*/
UCLASS()
class REALTIMESTRATEGY_API ARTSRangeIndicator : public AActor
{
    GENERATED_BODY()

public:
    /** Gets the range currently indicated by this actor. */
    UFUNCTION(BlueprintPure, Category = RTS)
    float GetRange() const;

    /** Sets the range to indicate with this actor. */
    UFUNCTION(BlueprintCallable, Category = RTS)
    void SetRange(float InRange);

    /** Event when the range to indicate has changed. */
    virtual void NotifyOnRangeChanged(float NewRange);

    /** Event when the range to indicate has changed. */
    UFUNCTION(BlueprintImplementableEvent, Category = "RTS", meta = (DisplayName = "OnRangeChanged"))
    void ReceiveOnRangeChanged(float NewRange);

private:
    /** Range currently indicated by this actor. */
    float Range;
};
