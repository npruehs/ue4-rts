#include "RTSPluginPrivatePCH.h"
#include "RTSCharacterAIController.h"


void ARTSCharacterAIController::BeginPlay()
{
    Super::BeginPlay();

    // Make AI use assigned blackboard.
    UBlackboardComponent* BlackboardComponent;
    UseBlackboard(CharacterBlackboardAsset, BlackboardComponent);
}
