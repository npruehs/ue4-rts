// © Acinex Games 2021

#pragma once

#include "CoreMinimal.h"

#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "RTSMoveInRangeTask.generated.h"

/**
 * 
 */
UCLASS(DisplayName="Move In Range", Category="RTS")
class REALTIMESTRATEGY_API URTSMoveInRangeTask : public UBTTask_MoveTo
{
	GENERATED_BODY()
	URTSMoveInRangeTask(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category=Blackboard)
	FBlackboardKeySelector Range;

	virtual FString GetStaticDescription() const override;
};
