#pragma once

#include "CoreMinimal.h"

#include "GameFramework/CheatManager.h"
#include "Templates/SubclassOf.h"

#include "RTSCheatManager.generated.h"


class URTSResourceType;


UCLASS(Blueprintable, Within = PlayerController)
class REALTIMESTRATEGY_API URTSCheatManager : public UCheatManager
{
    GENERATED_BODY()

public:
    URTSCheatManager(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


    /** Increases construction and production speed. */
    UFUNCTION(exec, BlueprintCallable, Category = "Cheat Manager")
    virtual void Boost();

    /** Increase damage of all own units. */
    UFUNCTION(exec, BlueprintCallable, Category = "Cheat Manager")
    virtual void Damage();

    /** Invulnerability cheat. */
    virtual void God() override;

    /** Adds resources. */
    UFUNCTION(exec, BlueprintCallable, Category = "Cheat Manager")
    virtual void Money();

    /** Toggles fog of war on/off. */
    UFUNCTION(exec, BlueprintCallable, Category = "Cheat Manager")
    virtual void NoFog();

    /** Defeat all other players. */
    UFUNCTION(exec, BlueprintCallable, Category = "Cheat Manager")
    virtual void Victory();

    /** Decreases damage of all own units. */
    UFUNCTION(exec, BlueprintCallable, Category = "Cheat Manager")
    virtual void Weak();

private:
    /** Factor to multiply all damage dealt by actors owned by the player with. */
    UPROPERTY(EditDefaultsOnly, Category = "Cheat Manager")
    float OutgoingDamageFactor;

    /** Amount of resources to grant with the Money cheat. */
    UPROPERTY(EditDefaultsOnly, Category = "Cheat Manager")
    float ResourceAmount;

    /** Types of resources to grant with the Money cheat. */
    UPROPERTY(EditDefaultsOnly, Category = "Cheat Manager")
    TArray<TSubclassOf<URTSResourceType>> ResourceTypes;

    /** Factor to multiply all construction and production speed with. */
    UPROPERTY(EditDefaultsOnly, Category = "Cheat Manager")
    float SpeedBoostFactor;
};
