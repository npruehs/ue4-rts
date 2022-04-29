// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Behavior/Task/RTSBeginConstructionTask.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Construction/RTSBuilderComponent.h"
#include "GameFramework/Controller.h"

URTSBeginConstructionTask::URTSBeginConstructionTask(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "Begin Construction";
}

EBTNodeResult::Type URTSBeginConstructionTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	BuilderComponent->BeginConstructionByIndex(OwnerComp.GetBlackboardComponent()->GetValueAsInt(BuildingTypeKey.SelectedKeyName),
	                                           OwnerComp.GetBlackboardComponent()->GetValueAsVector(BlackboardKey.SelectedKeyName));

	return EBTNodeResult::Succeeded;
}

FString URTSBeginConstructionTask::GetStaticDescription() const
{
	FString Description;

	Description.Append(NodeName).Append("\n")
	           .Append("\n").Append("Building Type: ").Append(BuildingTypeKey.SelectedKeyName.ToString())
	           .Append("\n").Append("Target: ").Append(BlackboardKey.SelectedKeyName.ToString());

	return Description;
}
