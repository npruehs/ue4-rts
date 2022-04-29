// © Acinex Games 2021

#pragma once

#include "CoreMinimal.h"

#include "BehaviorTree/BTTaskNode.h"
#include "RTSContinueGatherTask.generated.h"

/**
 * 
 */
UCLASS(DisplayName="Continue Gather", Category="RTS")
class REALTIMESTRATEGY_API URTSContinueGatherTask : public UBTTaskNode
{
	GENERATED_BODY()
public:
	URTSContinueGatherTask(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
