#pragma once

#include "CoreMinimal.h"

#include "Components/WidgetComponent.h"

#include "RTSHealthBarWidgetComponent.generated.h"


class URTSHealthComponent;


/**
* Adds a health bar widget to the actor.
*/
UCLASS(Blueprintable)
class REALTIMESTRATEGY_API URTSHealthBarWidgetComponent : public UWidgetComponent
{
    GENERATED_BODY()

public:
    URTSHealthBarWidgetComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    virtual void BeginPlay() override;

    /** Event when the current health of the actor has changed. */
    UFUNCTION(BlueprintImplementableEvent, Category = "RTS")
    void UpdateHealthBar(float HealthPercentage);

    /** Event when an offset for the health bar has been calculated from the actor collision size. */
    UFUNCTION(BlueprintImplementableEvent, Category = "RTS")
    void UpdateYOffset(float NewYOffset);

    /** Event when a width for the health bar has been calculated from the actor collision size. */
    UFUNCTION(BlueprintImplementableEvent, Category = "RTS")
    void UpdateWidth(float NewWidth);

private:
    UPROPERTY()
    URTSHealthComponent* HealthComponent;

    UFUNCTION()
    void OnHealthChanged(float OldHealth, float NewHealth, AActor* DamageCauser);
};
