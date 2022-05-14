// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "UObject/Object.h"
#include "RTSBeginConstructionTask.generated.h"

/**
 * 
 */
UCLASS(DisplayName="Begin Construction", Category="RTS")
class REALTIMESTRATEGY_API URTSBeginConstructionTask : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	explicit URTSBeginConstructionTask(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual FString GetStaticDescription() const override;
	
protected:
	/** Building Type Key Selector */
	UPROPERTY(EditAnywhere, Category=Blackboard)
	FBlackboardKeySelector BuildingTypeKey;
};
