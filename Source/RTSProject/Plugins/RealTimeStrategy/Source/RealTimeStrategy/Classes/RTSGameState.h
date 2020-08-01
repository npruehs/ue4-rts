#pragma once

#include "CoreMinimal.h"

#include "GameFramework/GameStateBase.h"

#include "RTSGameState.generated.h"


class ARTSVisionManager;


/**
* Game state with RTS features.
*/
UCLASS()
class REALTIMESTRATEGY_API ARTSGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
    virtual void BeginPlay() override;
    
    /** Gets the manager for updating actor vision and visibility. */
    ARTSVisionManager* GetVisionManager() const;

private:
    /** Manager for updating actor vision and visibility. */
    UPROPERTY()
    ARTSVisionManager* VisionManager;
};
