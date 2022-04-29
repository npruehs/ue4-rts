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

	const URTSAttackComponent* AttackComponent = Pawn->FindComponentByClass<URTSAttackComponent>();
	if (!IsValid(AttackComponent))
	{
		UE_LOG(LogRTS, Error, TEXT("For %s was no RTSCombatComponent found"), *Pawn->GetName())
		return EBTNodeResult::Failed;
	}

	const float Range = AttackComponent->GetAttacks()[0].Range;

	UE_LOG(LogRTS, Log, TEXT("Set range to %f"), Range)

	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(GetSelectedBlackboardKey(), Range);

	return EBTNodeResult::Succeeded;
}
