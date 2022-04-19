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
		UE_LOG(LogRTS, Error, TEXT("Not an AI Controller"));
		return EBTNodeResult::Failed;
	}

	const TSubclassOf<AActor> NextPawnToProduce = PlayerAIController->GetNextPawnToProduce();
	if (!IsValid(NextPawnToProduce))
	{
		UE_LOG(LogRTS, Log, TEXT("Build order completed"));
		return EBTNodeResult::Failed;
	}

	if (!PlayerAIController->CanPayFor(NextPawnToProduce))
	{
		UE_LOG(LogRTS, Log, TEXT("AI has not enough resources for %s"), *NextPawnToProduce->GetName());
		return EBTNodeResult::Failed;
	}

	if (!PlayerAIController->MeetsAllRequirementsFor(NextPawnToProduce))
	{
		UE_LOG(LogRTS, Log, TEXT("AI does not meet all requirements for %s"), *NextPawnToProduce->GetName());
		return EBTNodeResult::Failed;
	}

	PlayerAIController->StartProduction(NextPawnToProduce);
	return EBTNodeResult::Succeeded;
}
