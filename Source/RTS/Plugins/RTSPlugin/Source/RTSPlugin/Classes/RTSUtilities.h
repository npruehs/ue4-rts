#pragma once

#include "RTSPluginPCH.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RTSUtilities.generated.h"


class AActor;
class UWorld;


/**
* Various utility functions.
*/
UCLASS()
class RTSPLUGIN_API URTSUtilities : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

public:
	/** Checks whether the specified actor is ready to use (e.g. finished construction). */
	static bool IsReadyToUse(AActor* Actor);

    /**
    * Checks whether the specified actor can be placed at the passed location.
    */
    UFUNCTION(BlueprintCallable, Category = "RTS")
    static bool IsSuitableLocationForActor(UWorld* World, TSubclassOf<AActor> ActorClass, const FVector& Location);

	/** Searches the components attached to the specified actor class and returns the first encountered component of the specified class. */
	template<class T>
	static T* FindDefaultComponentByClass(const TSubclassOf<AActor> InActorClass)
	{
		return (T*)FindDefaultComponentByClass(InActorClass, T::StaticClass());
	}

	/** Searches the components attached to the specified actor class and returns the first encountered component of the specified class. */
	UFUNCTION(BlueprintCallable, Category = "RTS")
	static UActorComponent* FindDefaultComponentByClass(const TSubclassOf<AActor> InActorClass, const TSubclassOf<UActorComponent> InComponentClass);
};
