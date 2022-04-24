// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GEMeleeDamage.generated.h"

/**
 * 
 */
UCLASS(Category="RTS")
class REALTIMESTRATEGY_API UGEMeleeDamage : public UGameplayEffect
{
	GENERATED_BODY()

public:
	explicit UGEMeleeDamage(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
