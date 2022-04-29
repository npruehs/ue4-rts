#pragma once

#include "CoreMinimal.h"

#include "Kismet/BlueprintFunctionLibrary.h"

#include "GameFramework/Actor.h"
#include "Templates/SubclassOf.h"

#include "RTSCollisionLibrary.generated.h"


class UObject;
class UShapeComponent;


/**
* Utility functions for calculating actor collisions.
*/
UCLASS(Category="RTS")
class REALTIMESTRATEGY_API URTSCollisionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** Gets the distance between the two specified actors, optionally subtracting their collision sizes. */
	UFUNCTION(BlueprintPure, Category = "RTS")
	static float GetActorDistance(AActor* First, AActor* Second, bool bConsiderCollisionSize);

	/** Gets the approximated collision size of actors of the specified class. */
	UFUNCTION(BlueprintPure, Category = "RTS")
	static float GetCollisionSize(TSubclassOf<AActor> ActorClass);

	/** Gets the approximated collision height of actors of the specified class. */
	UFUNCTION(BlueprintPure, Category = "RTS")
	static float GetCollisionHeight(TSubclassOf<AActor> ActorClass);

	/** Gets the approximated collision size of the specified actor. */
	UFUNCTION(BlueprintPure, Category = "RTS")
	static float GetActorCollisionSize(AActor* Actor);

	/** Gets the approximated collision height of the specified actor. */
	UFUNCTION(BlueprintPure, Category = "RTS")
	static float GetActorCollisionHeight(AActor* Actor);

	/** Gets the approximated collision size of the specified shape. */
	UFUNCTION(BlueprintPure, Category = "RTS")
	static float GetShapeCollisionSize(UShapeComponent* ShapeComponent);

	/** Gets the approximated collision height of the specified shape. */
	UFUNCTION(BlueprintPure, Category = "RTS")
	static float GetShapeCollisionHeight(UShapeComponent* ShapeComponent);

	/** Casts a ray to determine the z coordinate of the specified location on ground level. */
	UFUNCTION(BlueprintPure, Category = "RTS", meta = (WorldContext = "WorldContextObject"))
	static FVector GetGroundLocation(UObject* WorldContextObject, FVector Location);

	/**
	* Checks whether the specified actor can be placed at the passed location.
	*/
	UFUNCTION(BlueprintPure, Category = "RTS")
	static bool IsSuitableLocationForActor(UWorld* World, TSubclassOf<AActor> ActorClass, const FVector& Location);
};
