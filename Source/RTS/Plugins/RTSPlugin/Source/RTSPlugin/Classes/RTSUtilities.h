#pragma once

#include "RTSPluginPrivatePCH.h"

#include "RTSUtilities.generated.h"


class AActor;


/**
* Various utility functions.
*/
UCLASS()
class URTSUtilities : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

public:
	/** Checks whether the specified actor is ready to use (e.g. finished construction). */
	static bool IsReadyToUse(AActor* Actor);

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
