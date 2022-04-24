// © Acinex Games 2021

#include "Combat/Abilities/GECMeleeDamage.h"
#include "AbilitySystemComponent.h"
#include "Combat/RTSAttributeSet.h"

struct FMeleeDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Strength);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Endurance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);

	FMeleeDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(URTSAttributeSet, Strength, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URTSAttributeSet, Endurance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URTSAttributeSet, Damage, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URTSAttributeSet, Health, Target, false);
	}
};

static const FMeleeDamageStatics& MeleeDamageStatics()
{
	static FMeleeDamageStatics DamageStatics;
	return DamageStatics;
}

UGECMeleeDamage::UGECMeleeDamage(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	RelevantAttributesToCapture.Add(MeleeDamageStatics().HealthDef); //RelevantAttributesToCapture is the array that contains all attributes you wish to capture, without exceptions. 
	RelevantAttributesToCapture.Add(MeleeDamageStatics().DamageDef);
	RelevantAttributesToCapture.Add(MeleeDamageStatics().EnduranceDef);
	RelevantAttributesToCapture.Add(MeleeDamageStatics().StrengthDef);
}

void UGECMeleeDamage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	float BaseDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(MeleeDamageStatics().DamageDef, EvaluateParameters, BaseDamage);
	float Strength = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(MeleeDamageStatics().StrengthDef, EvaluateParameters, Strength);
	float Defense = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(MeleeDamageStatics().EnduranceDef, EvaluateParameters, Defense);

	Strength = FMath::Max(1.f, Strength);
	Defense = FMath::Max(1.f, Defense);

	const float DamageDone = FMath::Max(1.0f, BaseDamage * Strength / Defense) * -1;

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(MeleeDamageStatics().HealthProperty, EGameplayModOp::Additive, DamageDone));
}
