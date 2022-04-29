// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Behavior/Task/RTSSetConstructionRangeTask.h"

#include "RTSLog.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Construction/RTSBuilderComponent.h"
#include "GameFramework/Controller.h"

URTSSetConstructionRangeTask::URTSSetConstructionRangeTask(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	NodeName = "Set Construction Range";
}

EBTNodeResult::Type URTSSetConstructionRangeTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	const URTSBuilderComponent* BuilderComponent = Pawn->FindComponentByClass<URTSBuilderComponent>();
	if (!IsValid(BuilderComponent))
	{
		UE_LOG(LogRTS, Error, TEXT("For %s was no BuilderComponent found"), *Pawn->GetName())
		return EBTNodeResult::Failed;
	}

	const uint8 Index = OwnerComp.GetBlackboardComponent()->GetValueAsInt(BlackboardKey.SelectedKeyName);
	const float Range = BuilderComponent->GetConstructionRange(Index);

	if (Range <= 0.0F)
	{
		return EBTNodeResult::Failed;
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(RangeKey.SelectedKeyName, Range);

	return EBTNodeResult::Succeeded;
}

FString URTSSetConstructionRangeTask::GetStaticDescription() const
{
	FString Description;

	Description.Append(NodeName).Append("\n")
	           .Append("\n").Append("Range: ").Append(RangeKey.SelectedKeyName.ToString())
	           .Append("\n").Append("Building Type: ").Append(BlackboardKey.SelectedKeyName.ToString());

	return Description;
}
