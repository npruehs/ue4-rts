// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RTSRace.generated.h"

/**
 * 
 */
UCLASS()
class REALTIMESTRATEGY_API URTSRace : public UDataAsset
{
	GENERATED_BODY()

public:
	/** The name of the race */
	UPROPERTY(EditDefaultsOnly, Category="RTS")
	FText Name;

	/** Actors to spawn for each player in the game. */
	UPROPERTY(EditDefaultsOnly, Category = "RTS")
	TArray<TSubclassOf<AActor>> InitialActors;

	/** Relative locations of the actors to spawn for each player in the game, relative to their respective start spot. */
	UPROPERTY(EditDefaultsOnly, Category = "RTS")
	TArray<FVector> InitialActorLocations;

	/** Optional types of actors that are required for a player to be alive. As soon as no actor of the specified type is alive, the player is defeated. */
	UPROPERTY(EditDefaultsOnly, Category = "RTS")
	TArray<TSubclassOf<AActor>> DefeatConditionActorClasses;

	/** Units and buildings the AI should produce, in order. */
	UPROPERTY(EditDefaultsOnly, Category = "RTS")
	TArray<TSubclassOf<AActor>> BuildOrder;

	UPROPERTY(EditDefaultsOnly, Category="RTS")
	bool bIncludeInitialActorsInBuildOrder = true;

	UFUNCTION(BlueprintPure, Category = "RTS")
	TArray<TSubclassOf<AActor>> GetBuildOrder() const;
};
