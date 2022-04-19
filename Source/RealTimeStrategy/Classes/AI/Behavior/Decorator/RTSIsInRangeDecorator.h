// © Acinex Games 2021

#pragma once

#include "CoreMinimal.h"

#include "BehaviorTree/BTDecorator.h"
#include "RTSIsInRangeDecorator.generated.h"

/**
 * 
 */
UCLASS(DisplayName="Is In Range", Category="RTS")
class REALTIMESTRATEGY_API URTSIsInRangeDecorator : public UBTDecorator
{
	GENERATED_BODY()
public:
	URTSIsInRangeDecorator(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	virtual FString GetStaticDescription() const override;

	/** blackboard key selector */
	UPROPERTY(EditAnywhere, Category=Blackboard)
	FBlackboardKeySelector Target;
	/** blackboard key selector */
	UPROPERTY(EditAnywhere, Category=Blackboard)
	FBlackboardKeySelector Range;

	UPROPERTY(EditAnywhere, Category=Blackboard)
	float AcceptanceRadius;
};
