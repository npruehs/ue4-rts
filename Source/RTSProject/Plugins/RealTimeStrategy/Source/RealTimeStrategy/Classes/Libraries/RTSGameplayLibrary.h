#pragma once

#include "CoreMinimal.h"

#include "Kismet/BlueprintFunctionLibrary.h"

#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"
#include "Templates/SubclassOf.h"

#include "RTSGameplayLibrary.generated.h"

/**
* Utility functions for gameplay calculations.
*/
UCLASS()
class REALTIMESTRATEGY_API URTSGameplayLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /** Searches the components attached to the specified actor class and returns the first encountered component of the specified class. */
    template<class T>
    static T* FindDefaultComponentByClass(const TSubclassOf<AActor> InActorClass)
    {
        return (T*)FindDefaultComponentByClass(InActorClass, T::StaticClass());
    }

    /** Searches the components attached to the specified actor class and returns the first encountered component of the specified class. */
    UFUNCTION(BlueprintPure, Category = "RTS")
    static UActorComponent* FindDefaultComponentByClass(const TSubclassOf<AActor> InActorClass, const TSubclassOf<UActorComponent> InComponentClass);

    /** Checks whether the specified actor is owned by an AI player. */
    UFUNCTION(BlueprintPure, Category = "RTS")
    static bool IsAIUnit(AActor* Actor);

    /** Checks whether the specified actor is ready to use (e.g. finished construction). */
    UFUNCTION(BlueprintPure, Category = "RTS")
    static bool IsReadyToUse(AActor* Actor);

    /** Checks if the owner of the specified actor meets all requirements for producing the desired new actor. */
    UFUNCTION(BlueprintPure, Category = "RTS", meta = (WorldContext = "WorldContextObject"))
    static bool OwnerMeetsAllRequirementsFor(UObject* WorldContextObject, AActor* OwnedActor, TSubclassOf<AActor> DesiredProduct);

    /** Checks if the owner of the specified actor meets all requirements for producing the desired new actor. */
    static bool GetMissingRequirementFor(UObject* WorldContextObject, AActor* OwnedActor, TSubclassOf<AActor> DesiredProduct, TSubclassOf<AActor>& OutMissingRequirement);

private:
    static bool IsOwnerABot(class URTSOwnerComponent* OwnerComponent);
};
