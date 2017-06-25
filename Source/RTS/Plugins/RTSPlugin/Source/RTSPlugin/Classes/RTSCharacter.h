#pragma once

#include "RTSPluginPrivatePCH.h"

#include "GameFramework/Character.h"

#include "RTSAttackData.h"

#include "RTSCharacter.generated.h"


class URTSAttackComponent;


/**
* Character with RTS features, such as using an attack.
*/
UCLASS()
class ARTSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	/** Event when the attack cooldown has expired. */
	virtual void NotifyOnCooldownReady();

	/** Event when the current health of the character has changed. */
	virtual void NotifyOnHealthChanged(float OldHealth, float NewHealth);
	
	/** Event when the character has been killed. */
	virtual void NotifyOnKilled(AController* PreviousOwner);

	/** Event when a character has used an attack. */
	virtual void NotifyOnUsedAttack(const FRTSAttackData& Attack, AActor* Target);

	/** Event when the attack cooldown has expired. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS", meta = (DisplayName = "OnCooldownReady"))
	void ReceiveOnCooldownReady();

	/** Event when the current health of the character has changed. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS", meta = (DisplayName = "OnHealthChanged"))
	void ReceiveOnHealthChanged(float OldHealth, float NewHealth);

	/** Event when the character has been killed. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS", meta = (DisplayName = "OnKilled"))
	void ReceiveOnKilled(AController* PreviousOwner);

	/** Event when a character has used an attack. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS", meta = (DisplayName = "OnUsedAttack"))
	void ReceiveOnUsedAttack(const FRTSAttackData& Attack, AActor* Target);


	virtual void Tick(float DeltaSeconds) override;
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void BeginPlay() override;

private:
	/** Attack data of this character. */
	URTSAttackComponent* AttackComponent;


	/** Uses the passed attack on the specified target and starts the cooldown timer. */
	UFUNCTION(BlueprintCallable)
	void UseAttack(int AttackIndex, AActor* Target);
};
