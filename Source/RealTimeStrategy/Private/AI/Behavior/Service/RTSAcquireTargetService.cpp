// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Behavior/Service/RTSAcquireTargetService.h"

#include "RTSPawnAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Orders/RTSStopOrder.h"

URTSAcquireTargetService::URTSAcquireTargetService(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "Acquire Target";
}

void URTSAcquireTargetService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (OwnerComp.GetBlackboardComponent()->GetValueAsClass(TEXT("OrderClass")) != URTSStopOrder::StaticClass())
	{
		return;
	}

	ARTSPawnAIController* PawnAIController = Cast<ARTSPawnAIController>(OwnerComp.GetOwner());

	PawnAIController->FindTargetInAcquisitionRadius();
}
