// © Acinex Games 2021

#pragma once

#include "CoreMinimal.h"

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"

#include "SetAttackRangeTask.generated.h"

/**
 * 
 */
UCLASS(DisplayName="Set Attack Range", Category="RTS")
class REALTIMESTRATEGY_API USetAttackRangeTask : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	USetAttackRangeTask(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	/** Target key selector */
	UPROPERTY(EditAnywhere, Category=Blackboard)
	FBlackboardKeySelector TargetKey;
};
