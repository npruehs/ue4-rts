// © Acinex Games 2021

#include "AI/Behavior/Task/SetAttackRangeTask.h"

#include "RTSLog.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Combat/RTSAttackComponent.h"
#include "Combat/RTSCombatComponent.h"
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

	const URTSCombatComponent* CombatComponent = Pawn->FindComponentByClass<URTSCombatComponent>();
	if (!IsValid(CombatComponent))
	{
		const URTSAttackComponent* AttackComponent = Controller->GetPawn()->FindComponentByClass<URTSAttackComponent>();

		if (!IsValid(AttackComponent) || AttackComponent->GetAttacks().Num() == 0)
		{
			return EBTNodeResult::Failed;
		}
		/* TODO: Get suitable range per actor */
		AttackComponent->GetAttacks()[0].Range;
		return EBTNodeResult::Succeeded;
	}

	const float Range = CombatComponent->GetAttackRange(Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetKey.SelectedKeyName)));

	UE_LOG(LogRTS, Log, TEXT("Set range to %f"), Range)

	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(GetSelectedBlackboardKey(), Range);

	return EBTNodeResult::Succeeded;
}
