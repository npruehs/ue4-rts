#pragma once

#include "RealTimeStrategyPCH.h"

#include "Components/ActorComponent.h"

#include "RTSFloatingCombatTextData.h"

#include "RTSFloatingCombatTextComponent.generated.h"


/**
* Stores floating texts to be displayed above the actor.
*/
UCLASS(meta = (BlueprintSpawnableComponent))
class REALTIMESTRATEGY_API URTSFloatingCombatTextComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    /** Floating combat texts currently being displayed to the player. */
    TArray<FRTSFloatingCombatTextData> Texts;


    URTSFloatingCombatTextComponent(const FObjectInitializer& ObjectInitializer);


    virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;


    /** Adds the specified text to be displayed above the actor in the HUD for a short time. */
    UFUNCTION(BlueprintCallable)
    void AddText(const FString& Text, const FLinearColor& Color, float Scale, float Lifetime);
};
