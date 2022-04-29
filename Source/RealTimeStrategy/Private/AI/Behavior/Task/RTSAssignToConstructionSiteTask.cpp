// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Behavior/Task/RTSAssignToConstructionSiteTask.h"

#include "RTSLog.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Construction/RTSBuilderComponent.h"
#include "GameFramework/Controller.h"

URTSAssignToConstructionSiteTask::URTSAssignToConstructionSiteTask(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "Assign To Construction Site";
}

EBTNodeResult::Type URTSAssignToConstructionSiteTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
		UE_LOG(LogRTS, Error, TEXT("For %s was no BuilderComponent found"), *Pawn->GetName())
		return EBTNodeResult::Failed;
	}

	AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BlackboardKey.SelectedKeyName));

	BuilderComponent->AssignToConstructionSite(Target);

	return EBTNodeResult::Succeeded;
}

FString URTSAssignToConstructionSiteTask::GetStaticDescription() const
{
	FString Description;

	Description.Append(NodeName).Append("\n")
	           .Append("\n").Append("Target: ").Append(BlackboardKey.SelectedKeyName.ToString());

	return Description;
}
