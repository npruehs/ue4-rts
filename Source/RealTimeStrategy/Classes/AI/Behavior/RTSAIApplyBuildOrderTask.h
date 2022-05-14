// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RTSAIApplyBuildOrderTask.generated.h"

/**
 * 
 */
UCLASS(DisplayName="AI Apply Build Order", Category="RTS")
class REALTIMESTRATEGY_API URTSAIApplyBuildOrderTask : public UBTTaskNode
{
	GENERATED_BODY()

public:
	explicit URTSAIApplyBuildOrderTask(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
