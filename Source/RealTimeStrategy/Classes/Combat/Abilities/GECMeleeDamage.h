// © Acinex Games 2021

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GECMeleeDamage.generated.h"

/**
 * 
 */
UCLASS(Category="RTS")
class REALTIMESTRATEGY_API UGECMeleeDamage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	UGECMeleeDamage(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
