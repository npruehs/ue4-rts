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
	/** Event when a character has used an attack. */
	virtual void NotifyOnUsedAttack(const FRTSAttackData& Attack, AActor* Target);

	/** Event when the attack cooldown has expired. */
	virtual void NotifyOnCooldownReady();

	/** Event when a character has used an attack. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS", meta = (DisplayName = "OnUsedAttack"))
	void ReceiveOnUsedAttack(const FRTSAttackData& Attack, AActor* Target);

	/** Event when the attack cooldown has expired. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS", meta = (DisplayName = "OnCooldownReady"))
	void ReceiveOnCooldownReady();

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
