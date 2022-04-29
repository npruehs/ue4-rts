// © Acinex Games 2021

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "RTSHasGameplayTagDecorator.generated.h"

/**
 * 
 */
UCLASS(DisplayName="Has Gameplay Tag", Category="RTS")
class REALTIMESTRATEGY_API URTSHasGameplayTagDecorator : public UBTDecorator
{
	GENERATED_BODY()
public:
	URTSHasGameplayTagDecorator(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	virtual FString GetStaticDescription() const override;

protected:
	UPROPERTY(EditAnywhere, Category=Blackboard)
	FGameplayTag Tag;
};
