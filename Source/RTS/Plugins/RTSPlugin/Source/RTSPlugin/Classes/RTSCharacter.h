#pragma once

#include "RTSPluginPrivatePCH.h"

#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"

#include "RTSAttackData.h"

#include "RTSCharacter.generated.h"


class UDecalComponent;


class ARTSProjectile;
class URTSAttackComponent;
class URTSHealthComponent;


/**
* Character with RTS features, such as using an attack.
*/
UCLASS()
class ARTSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ARTSCharacter();


	/** Uses the passed attack on the specified target and starts the cooldown timer. */
	UFUNCTION(BlueprintCallable)
	void UseAttack(int AttackIndex, AActor* Target);


	/** Event when the attack cooldown has expired. */
	virtual void NotifyOnCooldownReady();

	/** Event when the character has been deselected. */
	virtual void NotifyOnDeselected();

	/** Event when the current health of the character has changed. */
	virtual void NotifyOnHealthChanged(float OldHealth, float NewHealth);
	
	/** Event when the character has been killed. */
	virtual void NotifyOnKilled(AController* PreviousOwner);

	/** Event when the character has been selected. */
	virtual void NotifyOnSelected();

	/** Event when a character has used an attack. */
	virtual void NotifyOnUsedAttack(const FRTSAttackData& Attack, AActor* Target, ARTSProjectile* Projectile);

	/** Event when the attack cooldown has expired. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS", meta = (DisplayName = "OnCooldownReady"))
	void ReceiveOnCooldownReady();

	/** Event when the character has been deselected. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS", meta = (DisplayName = "OnDeselected"))
	void ReceiveOnDeselected();

	/** Event when the current health of the character has changed. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS", meta = (DisplayName = "OnHealthChanged"))
	void ReceiveOnHealthChanged(float OldHealth, float NewHealth);

	/** Event when the character has been killed. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS", meta = (DisplayName = "OnKilled"))
	void ReceiveOnKilled(AController* PreviousOwner);

	/** Event when the character has been selected. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS", meta = (DisplayName = "OnSelected"))
	void ReceiveOnSelected();

	/** Event when a character has used an attack. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS", meta = (DisplayName = "OnUsedAttack"))
	void ReceiveOnUsedAttack(const FRTSAttackData& Attack, AActor* Target, ARTSProjectile* Projectile);


	virtual void Tick(float DeltaSeconds) override;
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void BeginPlay() override;

private:
	/** Attack data of this character. */
	URTSAttackComponent* AttackComponent;

	/** Health data of this character. */
	URTSHealthComponent* HealthComponent;

	/** Decal used for rendering the selection circle of this character. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UDecalComponent* SelectionCircleDecalComponent;

	/** Whether this unit is currently selected by the local player, or not. */
	bool bSelected;
};
