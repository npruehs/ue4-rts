// © Acinex Games 2021

#pragma once

#include "CoreMinimal.h"

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "RTSFaceTargetTask.generated.h"

/**
 * 
 */
UCLASS(DisplayName="Face Target", Category="RTS")
class REALTIMESTRATEGY_API URTSFaceTargetTask : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	URTSFaceTargetTask(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual FString GetStaticDescription() const override;
};
