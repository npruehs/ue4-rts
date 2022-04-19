#include "Combat/RTSAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "RTSLog.h"
#include "Combat/RTSCombatComponent.h"
#include "Net/UnrealNetwork.h"


void URTSAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
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

void URTSAttributeSet::Tick(float DeltaTime)
{
	const AActor* Owner = Cast<AActor>(GetOuter());
	URTSCombatComponent* CombatComponent = Owner->FindComponentByClass<URTSCombatComponent>();
	if (GetHealth() <= 0)
	{
		CombatComponent->KillActor(nullptr);
	}
}
