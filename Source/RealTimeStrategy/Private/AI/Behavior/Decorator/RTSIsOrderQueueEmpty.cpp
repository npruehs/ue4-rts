// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Behavior/Decorator/RTSIsOrderQueueEmpty.h"

#include "RTSPawnAIController.h"
#include "Libraries/RTSFunctionLibrary.h"

URTSIsOrderQueueEmpty::URTSIsOrderQueueEmpty(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	NodeName = "Is Order Queue Empty";
}

bool URTSIsOrderQueueEmpty::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const ARTSPawnAIController* Controller = Cast<ARTSPawnAIController>(OwnerComp.GetOwner());
	if (!IsValid(Controller))
	{
		return false;
	}

	return !Controller->HasOrderQueue();
}

FString URTSIsOrderQueueEmpty::GetStaticDescription() const
{
	return URTSFunctionLibrary::CreateDecoratorDescription(NodeName, {}, GetFlowAbortMode(), IsInversed());
}
