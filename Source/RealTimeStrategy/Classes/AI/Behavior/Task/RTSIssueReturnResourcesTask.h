// © Acinex Games 2021

#pragma once

#include "CoreMinimal.h"

#include "BehaviorTree/BTTaskNode.h"
#include "RTSIssueReturnResourcesTask.generated.h"

/**
 * 
 */
UCLASS(DisplayName="Issue Return Resources", Category="RTS")
class REALTIMESTRATEGY_API URTSIssueReturnResourcesTask : public UBTTaskNode
{
	GENERATED_BODY()
public:
	URTSIssueReturnResourcesTask(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
