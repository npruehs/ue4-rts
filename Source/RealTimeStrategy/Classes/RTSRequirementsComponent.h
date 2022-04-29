#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "Templates/SubclassOf.h"

#include "RTSRequirementsComponent.generated.h"


class AActor;


/** Adds tech tree requirements to the actor. */
UCLASS(ClassGroup="RTS", Category="RTS", meta = (BlueprintSpawnableComponent))
class REALTIMESTRATEGY_API URTSRequirementsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/** Gets the types of actors the player needs to own in order to create this actor. */
	UFUNCTION(BlueprintPure)
	TArray<TSubclassOf<AActor>> GetRequiredActors() const;

private:
	/** Types of actors the player needs to own in order to create this actor. */
	UPROPERTY(EditDefaultsOnly, Category = "RTS")
	TArray<TSubclassOf<AActor>> RequiredActors;
};
