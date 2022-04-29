// © Acinex Games 2021

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RTSWaitRandomTask.generated.h"

/**
 * 
 */
UCLASS(DisplayName="Wait Random")
class REALTIMESTRATEGY_API URTSWaitRandomTask : public UBTTaskNode
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
