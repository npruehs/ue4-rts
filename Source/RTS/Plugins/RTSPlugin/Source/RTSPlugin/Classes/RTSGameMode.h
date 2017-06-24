#pragma once

#include "RTSPluginPrivatePCH.h"

#include "GameFramework/GameModeBase.h"

#include "RTSGameMode.generated.h"

/**
* Game mode with RTS features, such as spawning initial units for each player.
*/
UCLASS()
class ARTSGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	/** Actors to spawn for each player in the game. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS")
	TArray<TSubclassOf<AActor>> InitialActors;


	virtual void RestartPlayerAtPlayerStart(AController* NewPlayer, AActor* StartSpot) override;
};
