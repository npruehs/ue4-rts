// © Acinex Games 2021

#include "AI/Behavior/Task/RTSLeaveConstructionSite.h"
#include "Construction/RTSBuilderComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"

URTSLeaveConstructionSite::URTSLeaveConstructionSite(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Leave Construction Site";
}

EBTNodeResult::Type URTSLeaveConstructionSite::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	URTSBuilderComponent* BuilderComponent = Pawn->FindComponentByClass<URTSBuilderComponent>();
	if (!IsValid(BuilderComponent))
	{
		return EBTNodeResult::Failed;
	}

	BuilderComponent->LeaveConstructionSite();

	return EBTNodeResult::Succeeded;
}
