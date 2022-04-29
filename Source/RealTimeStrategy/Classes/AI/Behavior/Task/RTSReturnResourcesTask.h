// © Acinex Games 2021

#pragma once

#include "CoreMinimal.h"

#include "BehaviorTree/BTTaskNode.h"
#include "RTSReturnResourcesTask.generated.h"

/**
 * 
 */
UCLASS(DisplayName="Return Resources", Category="RTS")
class REALTIMESTRATEGY_API URTSReturnResourcesTask : public UBTTaskNode
{
	GENERATED_BODY()
public:
	URTSReturnResourcesTask(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, Category=Blackboard)
	FBlackboardKeySelector ResourceDrain;
};
