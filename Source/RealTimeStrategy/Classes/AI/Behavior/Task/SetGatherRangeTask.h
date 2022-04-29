// © Acinex Games 2021

#pragma once

#include "CoreMinimal.h"

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "SetGatherRangeTask.generated.h"

/**
 * 
 */
UCLASS(DisplayName="Set Gather Range", Category="RTS")
class REALTIMESTRATEGY_API URTSSetGatherRangeTask : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	URTSSetGatherRangeTask(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual FString GetStaticDescription() const override;

protected:
	/** blackboard key selector */
	UPROPERTY(EditAnywhere, Category=Blackboard)
	FBlackboardKeySelector RangeKey;
};
