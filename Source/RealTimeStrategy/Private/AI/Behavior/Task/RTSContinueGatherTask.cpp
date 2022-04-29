// © Acinex Games 2021

#include "AI/Behavior/Task/RTSContinueGatherTask.h"

#include "RTSPawnAIController.h"
#include "Economy/RTSGathererComponent.h"

URTSContinueGatherTask::URTSContinueGatherTask(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	NodeName = "Continue Gather";
}

EBTNodeResult::Type URTSContinueGatherTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	AActor* PreferredResourceSource = GathererComponent->GetPreferredResourceSource();
	if (!IsValid(PreferredResourceSource))
	{
		return EBTNodeResult::Failed;
	}

	Controller->IssueGatherOrder(PreferredResourceSource);

	return EBTNodeResult::Succeeded;
}
