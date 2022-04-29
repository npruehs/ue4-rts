#pragma once

#include "CoreMinimal.h"

#include "GameFramework/FloatingPawnMovement.h"

#include "RTSPawnMovementComponent.generated.h"


/** Simple pawn movement that also updates rotation. */
UCLASS(meta = (BlueprintSpawnableComponent))
class REALTIMESTRATEGY_API URTSPawnMovementComponent : public UFloatingPawnMovement
{
    GENERATED_BODY()

public:
    URTSPawnMovementComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    virtual void UpdateComponentVelocity() override;

private:
    /** Whether to automatically update the pawn rotation to match its velocity. */
	UPROPERTY(EditDefaultsOnly, Category = "RTS")
	bool bUpdateRotation;
};
