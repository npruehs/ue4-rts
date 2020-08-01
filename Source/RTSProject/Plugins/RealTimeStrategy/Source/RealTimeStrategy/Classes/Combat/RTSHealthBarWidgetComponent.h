#pragma once

#include "CoreMinimal.h"

#include "UI/RTSActorWidgetComponent.h"

#include "RTSHealthBarWidgetComponent.generated.h"


class URTSHealthComponent;


/**
* Adds a health bar widget to the actor.
*/
UCLASS(Blueprintable)
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
    URTSHealthComponent* HealthComponent;

    UFUNCTION()
    void OnHealthChanged(AActor* Actor, float OldHealth, float NewHealth, AActor* DamageCauser);
};
