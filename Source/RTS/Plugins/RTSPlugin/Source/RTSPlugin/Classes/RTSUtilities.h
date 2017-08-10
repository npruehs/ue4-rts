#pragma once

#include "RTSPluginPrivatePCH.h"

#include "RTSUtilities.generated.h"


/**
* Various utility functions.
*/
UCLASS()
class URTSUtilities : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

public:
	/** Searches the components attached to the specified actor class and returns the first encountered component of the specified class. */
	template<class T>
	static T* FindDefaultComponentByClass(const TSubclassOf<AActor> InActorClass);

	/** Searches the components attached to the specified actor class and returns the first encountered component of the specified class. */
	UFUNCTION(BlueprintCallable, Category = "RTS")
	static UActorComponent* FindDefaultComponentByClass(const TSubclassOf<AActor> InActorClass, const TSubclassOf<UActorComponent> InComponentClass);
};
