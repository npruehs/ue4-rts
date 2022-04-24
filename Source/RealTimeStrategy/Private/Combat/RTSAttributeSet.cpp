#include "Combat/RTSAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Combat/RTSCombatComponent.h"
#include "Net/UnrealNetwork.h"


void URTSAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	const FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
	const UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent();

	// Compute the delta between old and new, if it is available
	float DeltaValue = 0;
	if (Data.EvaluatedData.ModifierOp == EGameplayModOp::Type::Additive)
	{
		// If this was additive, store the raw delta value to be passed along later
		DeltaValue = Data.EvaluatedData.Magnitude;
	}

	// Get the Target actor, which should be our owner
	AActor* TargetActor = nullptr;
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
	}

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));

		// Get the Source actor
		AActor* SourceActor = nullptr;
		if (Source && Source->AbilityActorInfo.IsValid() && Source->AbilityActorInfo->AvatarActor.IsValid())
		{
			SourceActor = Source->AbilityActorInfo->AvatarActor.Get();

			// Set the causer actor based on context if it's set
			if (Context.GetEffectCauser())
			{
				SourceActor = Context.GetEffectCauser();
			}
		}

		Cast<URTSCombatComponent>(Data.Target.AbilityActorInfo->AbilitySystemComponent.Get())->NotifyOnHealthChanged(TargetActor, DeltaValue, GetHealth(), SourceActor);
	}
}

void URTSAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URTSAttributeSet, Health)
	DOREPLIFETIME(URTSAttributeSet, MaxHealth)
	DOREPLIFETIME(URTSAttributeSet, Strength)
	DOREPLIFETIME(URTSAttributeSet, Endurance)
	DOREPLIFETIME(URTSAttributeSet, Damage)
	DOREPLIFETIME(URTSAttributeSet, MovementSpeed)
}

void URTSAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URTSAttributeSet, Health, OldHealth)
}

void URTSAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URTSAttributeSet, MaxHealth, OldMaxHealth)
}

void URTSAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URTSAttributeSet, Strength, OldStrength)
}

void URTSAttributeSet::OnRep_Endurance(const FGameplayAttributeData& OldEndurance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URTSAttributeSet, Endurance, OldEndurance)
}

void URTSAttributeSet::OnRep_WeaponDamage(const FGameplayAttributeData& OldWeaponDamage)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URTSAttributeSet, Damage, OldWeaponDamage)
}

void URTSAttributeSet::OnRep_MovementSpeed(const FGameplayAttributeData& OldMovementSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URTSAttributeSet, MovementSpeed, OldMovementSpeed)
}
