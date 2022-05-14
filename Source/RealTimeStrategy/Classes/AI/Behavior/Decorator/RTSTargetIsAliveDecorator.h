// © Acinex Games 2021

#pragma once

#include "CoreMinimal.h"

#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"

#include "RTSTargetIsAliveDecorator.generated.h"

/**
 * 
 */
UCLASS(DisplayName="Is Target Alive", Category="RTS")
class REALTIMESTRATEGY_API URTSTargetIsAliveDecorator : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
public:
	URTSTargetIsAliveDecorator(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	virtual FString GetStaticDescription() const override;
};
