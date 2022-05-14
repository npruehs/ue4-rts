// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "UObject/Object.h"
#include "RTSAssignToConstructionSiteTask.generated.h"

/**
 * 
 */
UCLASS(DisplayName="Assign To Construction Site", Category="RTS")
class REALTIMESTRATEGY_API URTSAssignToConstructionSiteTask : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	explicit URTSAssignToConstructionSiteTask(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual FString GetStaticDescription() const override;
};
