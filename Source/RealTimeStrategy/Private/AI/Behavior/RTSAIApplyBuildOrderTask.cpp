// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Behavior/RTSAIApplyBuildOrderTask.h"

#include "RTSLog.h"
#include "RTSPlayerAIController.h"

URTSAIApplyBuildOrderTask::URTSAIApplyBuildOrderTask(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	NodeName = "AI Apply Build Order";
}

EBTNodeResult::Type URTSAIApplyBuildOrderTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ARTSPlayerAIController* PlayerAIController = Cast<ARTSPlayerAIController>(OwnerComp.GetOwner());

	if (!IsValid(PlayerAIController))
	{
		return EBTNodeResult::Failed;
	}

	const TSubclassOf<AActor> NextPawnToProduce = PlayerAIController->GetNextPawnToProduce();
	if (!IsValid(NextPawnToProduce))
	{
		return EBTNodeResult::Failed;
	}

	if (!PlayerAIController->CanPayFor(NextPawnToProduce))
	{
		return EBTNodeResult::Failed;
	}

	if (!PlayerAIController->MeetsAllRequirementsFor(NextPawnToProduce))
	{
		return EBTNodeResult::Failed;
	}

	PlayerAIController->StartProduction(NextPawnToProduce);
	return EBTNodeResult::Succeeded;
}
