// © Acinex Games 2021

#include "AI/Behavior/Task/RTSReturnResourcesTask.h"

#include "RTSLog.h"
#include "RTSPawnAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Economy/RTSGathererComponent.h"

URTSReturnResourcesTask::URTSReturnResourcesTask(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	NodeName = "Return Resources";
}

EBTNodeResult::Type URTSReturnResourcesTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const ARTSPawnAIController* Controller = Cast<ARTSPawnAIController>(OwnerComp.GetOwner());
	if (!IsValid(Controller))
	{
		return EBTNodeResult::Failed;
	}

	const APawn* Pawn = Controller->GetPawn();
	if (!IsValid(Pawn))
	{
		return EBTNodeResult::Failed;
	}

	URTSGathererComponent* GathererComponent = Pawn->FindComponentByClass<URTSGathererComponent>();
	if (!IsValid(GathererComponent))
	{
		return EBTNodeResult::Failed;
	}

	AActor* Drain = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ResourceDrain.SelectedKeyName));
	UE_LOG(LogRTS, Log, TEXT("Returning Resources to %s"), Drain ? *Drain->GetName(): TEXT("null"))
	GathererComponent->ReturnResources(Drain);

	return EBTNodeResult::Succeeded;
}
