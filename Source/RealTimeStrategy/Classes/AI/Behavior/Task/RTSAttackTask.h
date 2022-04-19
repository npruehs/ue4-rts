// © Acinex Games 2021

#pragma once

#include "CoreMinimal.h"

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "RTSAttackTask.generated.h"

/**
 * 
 */
UCLASS(DisplayName="Attack", Category="RTS")
class REALTIMESTRATEGY_API URTSAttackTask : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	public:
	URTSAttackTask(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual FString GetStaticDescription() const override;
};
