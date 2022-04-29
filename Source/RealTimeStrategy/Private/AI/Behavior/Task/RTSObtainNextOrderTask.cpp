// © Acinex Games 2021

#include "AI/Behavior/Task/RTSObtainNextOrderTask.h"
#include "RTSPawnAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Orders/RTSStopOrder.h"

URTSObtainNextOrderTask::URTSObtainNextOrderTask(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	NodeName = "Obtain Next Order";
}

EBTNodeResult::Type URTSObtainNextOrderTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{	
	ARTSPawnAIController* Controller = Cast<ARTSPawnAIController>(OwnerComp.GetOwner());
	if (!IsValid(Controller))
	{
		return EBTNodeResult::Failed;
	}

	Controller->ObtainNextOrder();

	return EBTNodeResult::Succeeded;
}
