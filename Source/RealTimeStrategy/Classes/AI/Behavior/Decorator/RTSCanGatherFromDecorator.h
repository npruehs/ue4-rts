// © Acinex Games 2021

#pragma once

#include "CoreMinimal.h"

#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "RTSCanGatherFromDecorator.generated.h"

/**
 * 
 */
UCLASS(DisplayName="Can Gather From", Category="RTS")
class REALTIMESTRATEGY_API URTSCanGatherFromDecorator : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
	
public:
	URTSCanGatherFromDecorator(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	virtual FString GetStaticDescription() const override;
};
