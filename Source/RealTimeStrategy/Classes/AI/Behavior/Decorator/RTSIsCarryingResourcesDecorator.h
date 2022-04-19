// © Acinex Games 2021

#pragma once

#include "CoreMinimal.h"

#include "BehaviorTree/BTDecorator.h"
#include "RTSIsCarryingResourcesDecorator.generated.h"

/**
 * 
 */
UCLASS(DisplayName="Is Carrying Resources", Category="RTS")
class REALTIMESTRATEGY_API URTSIsCarryingResourcesDecorator : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	URTSIsCarryingResourcesDecorator(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	virtual FString GetStaticDescription() const override;
};
