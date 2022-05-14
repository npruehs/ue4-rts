// © Acinex Games 2021

#include "AI/Behavior/Task/RTSGatherResourcesTask.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Economy/RTSGathererComponent.h"
#include "GameFramework/Controller.h"

URTSGatherResourcesTask::URTSGatherResourcesTask(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	NodeName = "Gather Resources";
}

EBTNodeResult::Type URTSGatherResourcesTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	URTSGathererComponent* GathererComponent = Pawn->FindComponentByClass<URTSGathererComponent>();
	if (!IsValid(GathererComponent))
	{
		return EBTNodeResult::Failed;
	}

	AActor* Actor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BlackboardKey.SelectedKeyName));

	GathererComponent->StartGatheringResources(Actor);

	return EBTNodeResult::Succeeded;
}

FString URTSGatherResourcesTask::GetStaticDescription() const
{
	FString Description;

	Description.Append(NodeName).Append("\n")
	           .Append("\n").Append("Resource: ").Append(GetSelectedBlackboardKey().ToString());

	return Description;
}
