// © Acinex Games 2021

#pragma once

#include "CoreMinimal.h"

#include "BehaviorTree/BTTaskNode.h"
#include "RTSObtainNextOrderTask.generated.h"

/**
 * 
 */
UCLASS(DisplayName="Obtain Next Order", Category="RTS")
class REALTIMESTRATEGY_API URTSObtainNextOrderTask : public UBTTaskNode
{
	GENERATED_BODY()
public:
	URTSObtainNextOrderTask(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
