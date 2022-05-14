// © Acinex Games 2021

#include "AI/Behavior/Task/SetGatherRangeTask.h"

#include "RTSLog.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Economy/RTSGathererComponent.h"
#include "GameFramework/Controller.h"

URTSSetGatherRangeTask::URTSSetGatherRangeTask(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	NodeName = "Set Gather Range";
}

EBTNodeResult::Type URTSSetGatherRangeTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	const URTSGathererComponent* GathererComponent = Pawn->FindComponentByClass<URTSGathererComponent>();
	if (!IsValid(GathererComponent))
	{
		UE_LOG(LogRTS, Error, TEXT("For %s was no GatherComponent found"), *Pawn->GetName())
		return EBTNodeResult::Failed;
	}

	AActor*     Actor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BlackboardKey.SelectedKeyName));
	const float Range = GathererComponent->GetGatherRange(Actor);

	if (Range <= 0.0F)
	{
		return EBTNodeResult::Failed;
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(RangeKey.SelectedKeyName, Range);

	return EBTNodeResult::Succeeded;
}

FString URTSSetGatherRangeTask::GetStaticDescription() const
{
	FString Description;

	Description.Append(NodeName).Append("\n")
	           .Append("\n").Append("Range: ").Append(RangeKey.SelectedKeyName.ToString())
	           .Append("\n").Append("Target: ").Append(BlackboardKey.SelectedKeyName.ToString());

	return Description;
}
