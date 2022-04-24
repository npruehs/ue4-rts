// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RTSFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS(Category="RTS")
class REALTIMESTRATEGY_API URTSFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static FString CreateDecoratorDescription(const FString Name, TMap<FString, FString> Values, const EBTFlowAbortMode::Type AbortMode, bool bInverted);
};
