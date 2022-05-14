// © Acinex Games 2021

#include "AI/Behavior/Task/SetAttackRangeTask.h"

#include "RTSLog.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Combat/RTSAttackComponent.h"
#include "GameFramework/Controller.h"

USetAttackRangeTask::USetAttackRangeTask(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	NodeName = "Set Attack Range";
}

EBTNodeResult::Type USetAttackRangeTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AController* Controller = Cast<AController>(OwnerComp.GetOwner());
	if (!IsValid(Controller))
	{
		return EBTNodeResult::Failed;
	}

	const APawn* Pawn = Controller->GetPawn();
	if (!IsValid(Pawn))
	{
		return EBTNodeResult::Failed;
	}

	const URTSAttackComponent* AttackComponent = Controller->GetPawn()->FindComponentByClass<URTSAttackComponent>();
	if (!IsValid(AttackComponent))
	{
		return EBTNodeResult::Failed;
	}

	const float Range = AttackComponent->GetAttacks()[0].Range;

	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(GetSelectedBlackboardKey(), Range);

	return EBTNodeResult::Succeeded;
}
