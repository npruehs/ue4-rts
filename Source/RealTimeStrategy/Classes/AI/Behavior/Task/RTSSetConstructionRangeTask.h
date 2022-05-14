// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "UObject/Object.h"
#include "RTSSetConstructionRangeTask.generated.h"

/**
 * 
 */
UCLASS(DisplayName="Set Construction Range", Category="RTS")
class REALTIMESTRATEGY_API URTSSetConstructionRangeTask : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	explicit URTSSetConstructionRangeTask(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual FString GetStaticDescription() const override;
	
protected:
	/** blackboard key selector */
	UPROPERTY(EditAnywhere, Category=Blackboard)
	FBlackboardKeySelector RangeKey;
};
