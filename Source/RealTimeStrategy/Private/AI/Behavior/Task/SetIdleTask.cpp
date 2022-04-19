// © Acinex Games 2021

#include "AI/Behavior/Task/SetIdleTask.h"

#include "RTSPawnAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Orders/RTSStopOrder.h"

EBTNodeResult::Type URTSSetIdleTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ARTSPawnAIController* PawnAIController = Cast<ARTSPawnAIController>(OwnerComp.GetOwner());
	if(!IsValid(PawnAIController))
	{
		return EBTNodeResult::Failed;
	}

	PawnAIController->IssueStopOrder();

	return EBTNodeResult::Succeeded;
}
