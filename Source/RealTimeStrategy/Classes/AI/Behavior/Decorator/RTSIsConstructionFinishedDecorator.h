// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "RTSIsConstructionFinishedDecorator.generated.h"

/**
 * 
 */
UCLASS(DisplayName="Is Construction Finished", Category="RTS")
class REALTIMESTRATEGY_API URTSIsConstructionFinishedDecorator : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
	
public:
	explicit URTSIsConstructionFinishedDecorator(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	virtual FString GetStaticDescription() const override;
};
