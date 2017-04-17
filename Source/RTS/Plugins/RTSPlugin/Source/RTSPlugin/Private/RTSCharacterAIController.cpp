#include "RTSPluginPrivatePCH.h"
#include "RTSCharacterAIController.h"

#include "BehaviorTree/BlackboardComponent.h"


void ARTSCharacterAIController::BeginPlay()
{
    Super::BeginPlay();

    // Make AI use assigned blackboard.
    UBlackboardComponent* BlackboardComponent;
    UseBlackboard(CharacterBlackboardAsset, BlackboardComponent);

    // Run behavior tree.
    RunBehaviorTree(CharacterBehaviorTreeAsset);
}

void ARTSCharacterAIController::IssueMoveOrder(const FVector& Location)
{
    Blackboard->SetValueAsVector("TargetLocation", Location);
}