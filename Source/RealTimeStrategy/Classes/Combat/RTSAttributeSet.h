// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "TickableAttributeSetInterface.h"
#include "RTSAttributeSet.generated.h"

/**
 * 
 */
UCLASS(Category = "RTS")
class REALTIMESTRATEGY_API URTSAttributeSet : public UAttributeSet, public ITickableAttributeSetInterface
{
	GENERATED_BODY()
public:
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Current Health */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	FGameplayAttributeData Health;
	GAMEPLAYATTRIBUTE_VALUE_GETTER(Health);
	GAMEPLAYATTRIBUTE_VALUE_SETTER(Health);
	GAMEPLAYATTRIBUTE_VALUE_INITTER(Health);
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(URTSAttributeSet, Health);

	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	FGameplayAttributeData MaxHealth;
	GAMEPLAYATTRIBUTE_VALUE_GETTER(MaxHealth);
	GAMEPLAYATTRIBUTE_VALUE_SETTER(MaxHealth);
	GAMEPLAYATTRIBUTE_VALUE_INITTER(MaxHealth);
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(URTSAttributeSet, MaxHealth);

	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	FGameplayAttributeData Strength;
	GAMEPLAYATTRIBUTE_VALUE_GETTER(Strength);

	UFUNCTION()
	virtual void OnRep_Strength(const FGameplayAttributeData& OldStrength);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	FGameplayAttributeData Endurance;
	GAMEPLAYATTRIBUTE_VALUE_GETTER(Endurance);

	UFUNCTION()
	virtual void OnRep_Endurance(const FGameplayAttributeData& OldEndurance);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	FGameplayAttributeData Damage;
	GAMEPLAYATTRIBUTE_VALUE_GETTER(Damage);

	UFUNCTION()
	virtual void OnRep_WeaponDamage(const FGameplayAttributeData& OldWeaponDamage);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	FGameplayAttributeData MovementSpeed;
	GAMEPLAYATTRIBUTE_VALUE_GETTER(MovementSpeed);
	GAMEPLAYATTRIBUTE_VALUE_SETTER(MovementSpeed);
	GAMEPLAYATTRIBUTE_VALUE_INITTER(MovementSpeed);
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(URTSAttributeSet, MovementSpeed);

	UFUNCTION()
	virtual void OnRep_MovementSpeed(const FGameplayAttributeData& OldMovementSpeed);

	virtual void Tick(float DeltaTime) override;

	virtual bool ShouldTick() const override { return IsValid(GetOuter()); }
};
