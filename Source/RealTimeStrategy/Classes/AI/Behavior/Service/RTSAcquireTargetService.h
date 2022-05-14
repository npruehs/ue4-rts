// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "RTSAcquireTargetService.generated.h"

/**
 * 
 */
UCLASS(DisplayName="Acquire Target", Category="RTS")
class REALTIMESTRATEGY_API URTSAcquireTargetService : public UBTService
{
	GENERATED_BODY()
	
public:
	explicit URTSAcquireTargetService(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
