// © Acinex Games 2021

#include "AI/Behavior/Task/RTSMoveInRangeTask.h"
#include "BehaviorTree/BlackboardComponent.h"

URTSMoveInRangeTask::URTSMoveInRangeTask(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	NodeName = "Move In Range";
}

EBTNodeResult::Type URTSMoveInRangeTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AcceptableRadius = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(Range.SelectedKeyName);

	return Super::ExecuteTask(OwnerComp, NodeMemory);
}

FString URTSMoveInRangeTask::GetStaticDescription() const
{
	FString Description;

	Description.Append(NodeName).Append("\n")
	           .Append("\n").Append("AcceptableRadius: ").Append(Range.SelectedKeyName.ToString())
	           .Append("\n").Append("Target: ").Append(GetSelectedBlackboardKey().ToString());

	return Description;
}
