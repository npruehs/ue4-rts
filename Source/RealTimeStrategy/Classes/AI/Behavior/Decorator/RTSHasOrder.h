// © Acinex Games 2021

#pragma once

#include "CoreMinimal.h"

#include "BehaviorTree/BTDecorator.h"
#include "RTSHasOrder.generated.h"

class URTSOrder;
/**
 * 
 */
UCLASS(DisplayName="Has Order", Category="RTS")
class REALTIMESTRATEGY_API URTSHasOrder : public UBTDecorator
{
	GENERATED_BODY()

public:
	URTSHasOrder(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	virtual FString GetStaticDescription() const override;

protected:
	UPROPERTY(EditAnywhere, Category=Blackboard)
	TSubclassOf<URTSOrder> Order;
};
