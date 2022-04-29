// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "UObject/Object.h"
#include "RTSIsOrderQueueEmpty.generated.h"

/**
 * 
 */
UCLASS()
class REALTIMESTRATEGY_API URTSIsOrderQueueEmpty : public UBTDecorator
{
	GENERATED_BODY()

public:
	explicit URTSIsOrderQueueEmpty(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	virtual FString GetStaticDescription() const override;
};
