// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Abilities/GEMeleeDamage.h"

#include "Combat/Abilities/GECMeleeDamage.h"

UGEMeleeDamage::UGEMeleeDamage(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	FGameplayEffectExecutionDefinition Definition = {
		UGECMeleeDamage::StaticClass(),
		FGameplayTagContainer(),
		{},
		{},
		{}
	};

	Executions.Emplace(Definition);
}
