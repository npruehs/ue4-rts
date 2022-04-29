// © Acinex Games 2021

#include "AI/Behavior/Task/RTSIssueReturnResourcesTask.h"

#include "RTSPawnAIController.h"
#include "Economy/RTSGathererComponent.h"


URTSIssueReturnResourcesTask::URTSIssueReturnResourcesTask(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	NodeName = "Issue Return Resources";
}

EBTNodeResult::Type URTSIssueReturnResourcesTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ARTSPawnAIController* Controller = Cast<ARTSPawnAIController>(OwnerComp.GetOwner());
	if (!IsValid(Controller))
	{
		return EBTNodeResult::Failed;
	}

	const APawn* Pawn = Controller->GetPawn();
	if (!IsValid(Pawn))
	{
		return EBTNodeResult::Failed;
	}

	const URTSGathererComponent* GathererComponent = Pawn->FindComponentByClass<URTSGathererComponent>();
	if (!IsValid(GathererComponent))
	{
		return EBTNodeResult::Failed;
	}

	Controller->IssueReturnResourcesOrder();

	return EBTNodeResult::Succeeded;
}
