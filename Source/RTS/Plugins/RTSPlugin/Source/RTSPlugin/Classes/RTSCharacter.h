#pragma once

#include "RTSPluginPrivatePCH.h"

#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"

#include "RTSAttackData.h"

#include "RTSCharacter.generated.h"


class UDecalComponent;


/**
* Character with RTS features, such as using an attack.
*/
UCLASS()
class ARTSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ARTSCharacter();


	/** Event when the character has been deselected. */
	virtual void NotifyOnDeselected();

	/** Event when the character has been selected. */
	virtual void NotifyOnSelected();

	/** Event when the character has been deselected. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS", meta = (DisplayName = "OnDeselected"))
	void ReceiveOnDeselected();

	/** Event when the character has been selected. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS", meta = (DisplayName = "OnSelected"))
	void ReceiveOnSelected();


	virtual void Tick(float DeltaSeconds) override;
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void BeginPlay() override;

private:
	/** Decal used for rendering the selection circle of this character. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UDecalComponent* SelectionCircleDecalComponent;

	/** Whether this unit is currently selected by the local player, or not. */
	bool bSelected;
};
