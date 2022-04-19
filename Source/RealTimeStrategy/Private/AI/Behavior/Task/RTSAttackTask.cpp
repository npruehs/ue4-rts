// © Acinex Games 2021

#include "AI/Behavior/Task/RTSAttackTask.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Combat/RTSCombatComponent.h"
#include "GameFramework/Controller.h"

URTSAttackTask::URTSAttackTask(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	NodeName = "Attack";
}

EBTNodeResult::Type URTSAttackTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AController* Controller = Cast<AController>(OwnerComp.GetOwner());
	if (!IsValid(Controller))
	{
		return EBTNodeResult::Failed;
	}

	URTSCombatComponent* CombatComponent = Controller->GetPawn()->FindComponentByClass<URTSCombatComponent>();

	AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey()));

	if (!IsValid(CombatComponent) || !IsValid(Target))
	{
		return EBTNodeResult::Failed;
	}

	CombatComponent->UseAttack(0, Target);

	return EBTNodeResult::Succeeded;
}

FString URTSAttackTask::GetStaticDescription() const
{
	FString Description;

	Description.Append(NodeName).Append("\n")
	           .Append("\n").Append("Target: ").Append(GetSelectedBlackboardKey().ToString());

	return Description;
}
