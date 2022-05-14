// © Acinex Games 2021

#pragma once

#include "CoreMinimal.h"

#include "BehaviorTree/BTTaskNode.h"
#include "SetIdleTask.generated.h"

/**
 * 
 */
UCLASS(DisplayName="Set Idle", Category="RTS")
class REALTIMESTRATEGY_API URTSSetIdleTask : public UBTTaskNode
{
	GENERATED_BODY()

public:
	explicit URTSSetIdleTask(const FObjectInitializer& ObjectInitializer)
		: UBTTaskNode(ObjectInitializer)
	{
		NodeName = "Set Idle";
	}

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
