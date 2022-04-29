// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "RTSIsAtConstructionSiteDecorator.generated.h"

/**
 * 
 */
UCLASS(DisplayName="Is At Construction Site", Category="RTS")
class REALTIMESTRATEGY_API URTSIsAtConstructionSiteDecorator : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	explicit URTSIsAtConstructionSiteDecorator(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	virtual FString GetStaticDescription() const override;
};
