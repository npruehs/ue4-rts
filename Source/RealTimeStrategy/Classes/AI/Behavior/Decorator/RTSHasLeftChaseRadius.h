// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "UObject/Object.h"
#include "RTSHasLeftChaseRadius.generated.h"

/**
 * 
 */
UCLASS()
class REALTIMESTRATEGY_API URTSHasLeftChaseRadius : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()

public:
	explicit URTSHasLeftChaseRadius(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	virtual FString GetStaticDescription() const override;
};
