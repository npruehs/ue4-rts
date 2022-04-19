// © Acinex Games 2021

#pragma once

#include "CoreMinimal.h"

#include "BehaviorTree/BTTaskNode.h"
#include "RTSLeaveConstructionSite.generated.h"

/**
 * 
 */
UCLASS(DisplayName="Leave Construction Site", Category="RTS")
class REALTIMESTRATEGY_API URTSLeaveConstructionSite : public UBTTaskNode
{
	GENERATED_BODY()
public:
	URTSLeaveConstructionSite(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
