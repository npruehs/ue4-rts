#include "RTSPluginPrivatePCH.h"
#include "RTSCharacterAIController.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
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

void ARTSCharacterAIController::IssueAttackOrder(AActor* Target)
{
	IssueMoveOrder(Target->GetActorLocation());
}

void ARTSCharacterAIController::IssueMoveOrder(const FVector& Location)
{
    // Update blackboard.
    Blackboard->SetValueAsVector("TargetLocation", Location);

    // Stop any current orders and start over.
    UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
    if (BehaviorTreeComponent)
    {
        BehaviorTreeComponent->RestartTree();
    }
}

void ARTSCharacterAIController::IssueStopOrder()
{
	// Update blackboard.
	Blackboard->ClearValue("TargetLocation");

	// Stop any current orders and start over.
	UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BehaviorTreeComponent)
	{
		BehaviorTreeComponent->RestartTree();
	}
}
