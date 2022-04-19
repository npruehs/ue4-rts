// © Acinex Games 2021

#pragma once

#include "CoreMinimal.h"

#include "BehaviorTree/BTDecorator.h"
#include "RTSIsGatheringResourcesDecorator.generated.h"

/**
 * 
 */
UCLASS(DisplayName="Is Gathering Resources", Category="RTS")
class REALTIMESTRATEGY_API URTSIsGatheringResourcesDecorator : public UBTDecorator
{
	GENERATED_BODY()

public:
	URTSIsGatheringResourcesDecorator(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	virtual FString GetStaticDescription() const override;
};
