// Fill out your copyright notice in the Description page of Project Settings.


#include "Libraries/RTSFunctionLibrary.h"

FString URTSFunctionLibrary::CreateDecoratorDescription(const FString Name, TMap<FString, FString> Values, const EBTFlowAbortMode::Type AbortMode, const bool bInverted)
{
	FString FlowAbortDesc;
	if (AbortMode != EBTFlowAbortMode::None)
	{
		FlowAbortDesc = FString::Printf(TEXT("aborts %s"), *UBehaviorTreeTypes::DescribeFlowAbortMode(AbortMode).ToLower());
	}

	FString InversedDesc;
	if (bInverted)
	{
		InversedDesc = TEXT("inversed");
	}

	FString AdditionalDesc;
	if (FlowAbortDesc.Len() || InversedDesc.Len())
	{
		AdditionalDesc = FString::Printf(TEXT("( %s%s%s )\n"), *FlowAbortDesc,
										 FlowAbortDesc.Len() > 0 && InversedDesc.Len() > 0 ? TEXT(", ") : TEXT(""),
										 *InversedDesc);
	}

	FString Text = Name;

	if (Values.Num())
	{
		Text.Append(":\n");
	}

	for (auto Value : Values)
	{
		Text.Append("\n").Append(Value.Key).Append(": ").Append(Value.Value);
	}

	return FString::Printf(TEXT("%s%s"), *AdditionalDesc, *Text);
}
