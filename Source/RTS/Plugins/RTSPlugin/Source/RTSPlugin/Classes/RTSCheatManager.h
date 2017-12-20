#pragma once

#include "RTSPluginPCH.h"

#include "GameFramework/CheatManager.h"

#include "RTSCheatManager.generated.h"


class URTSResourceType;


UCLASS(Blueprintable, Within = PlayerController)
class RTSPLUGIN_API URTSCheatManager : public UCheatManager
{
    GENERATED_BODY()

public:
    /** Amount of resources to grant with the Money cheat. */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Cheat Manager")
    float ResourceAmount;

    /** Types of resources to grant with the Money cheat. */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Cheat Manager")
    TArray<TSubclassOf<URTSResourceType>> ResourceTypes;

    /** Factor to multiply all construction and production speed with. */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Cheat Manager")
    float SpeedBoostFactor;


    /** Increases construction and production speed. */
    UFUNCTION(exec, BlueprintCallable, Category = "Cheat Manager")
    virtual void Boost();

    /** Invulnerability cheat. */
    UFUNCTION(exec, BlueprintCallable, Category = "Cheat Manager")
    virtual void God() override;

    /** Adds resources. */
    UFUNCTION(exec, BlueprintCallable, Category = "Cheat Manager")
    virtual void Money();

    /** Defeat all other players. */
    UFUNCTION(exec, BlueprintCallable, Category = "Cheat Manager")
    virtual void Victory();
};
