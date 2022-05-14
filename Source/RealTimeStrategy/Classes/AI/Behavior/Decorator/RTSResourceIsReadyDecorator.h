// © Acinex Games 2021

#pragma once

#include "CoreMinimal.h"

#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "RTSResourceIsReadyDecorator.generated.h"

/**
 * 
 */
UCLASS(DisplayName="Is Resource Ready", Category="RTS")
class REALTIMESTRATEGY_API URTSResourceIsReadyDecorator : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()

public:
	URTSResourceIsReadyDecorator(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	virtual FString GetStaticDescription() const override;
};
