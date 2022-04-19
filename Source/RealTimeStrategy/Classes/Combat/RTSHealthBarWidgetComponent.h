#pragma once

#include "CoreMinimal.h"
#include "RTSHealthComponent.h"
#include "Components/ProgressBar.h"

#include "UI/RTSActorWidgetComponent.h"

#include "RTSHealthBarWidgetComponent.generated.h"


class URTSCombatComponent;


/**
* Adds a health bar widget to the actor.
*/
UCLASS(Blueprintable, Category = "RTS")
class REALTIMESTRATEGY_API URTSHealthBarWidgetComponent : public URTSActorWidgetComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	/** Event when the current health of the actor has changed. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS")
	void UpdateHealthBar(float HealthPercentage);

private:
	UPROPERTY()
	URTSCombatComponent* CombatComponent;
	UPROPERTY()
	URTSHealthComponent* HealthComponent;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
