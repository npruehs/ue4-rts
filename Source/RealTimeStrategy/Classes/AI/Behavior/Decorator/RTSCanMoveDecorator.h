// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "UObject/Object.h"
#include "RTSCanMoveDecorator.generated.h"

/**
 * 
 */
UCLASS(DisplayName="Can Move", Category="RTS")
class REALTIMESTRATEGY_API URTSCanMoveDecorator : public UBTDecorator
{
	GENERATED_BODY()
public:
	explicit URTSCanMoveDecorator(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	virtual FString GetStaticDescription() const override;
};
