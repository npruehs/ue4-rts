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

	if (const URTSGathererComponent* GathererComponent = Pawn->FindComponentByClass<URTSGathererComponent>(); !IsValid(GathererComponent))
	{
		return EBTNodeResult::Failed;
	}

	Controller->InsertContinueGathersOrder();

	return EBTNodeResult::Succeeded;
}
