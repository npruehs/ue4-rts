// © Acinex Games 2021

#pragma once

#include "CoreMinimal.h"

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "RTSGatherResourcesTask.generated.h"

/**
 * 
 */
UCLASS(DisplayName="Gather Resources", Category="RTS")
class REALTIMESTRATEGY_API URTSGatherResourcesTask : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	URTSGatherResourcesTask(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual FString GetStaticDescription() const override;
};
