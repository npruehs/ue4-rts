#pragma once

#include "RTSPluginPrivatePCH.h"

#include "GameFramework/GameModeBase.h"

#include "RTSGameMode.generated.h"


class ARTSCharacter;
class ARTSPlayerStart;


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

	/** Optional type of actor that is required for a player to be alive. As soon as no actor of the specified type is alive, the player is defeated. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS")
	TSubclassOf<AActor> DefeatConditionActor;

	ARTSPlayerStart* FindRTSPlayerStart(AController* Player);

	virtual void RestartPlayer(AController* NewPlayer) override;
	virtual void RestartPlayerAtPlayerStart(AController* NewPlayer, AActor* StartSpot) override;


	/** Event when a character has been killed. */
	virtual void NotifyOnCharacterKilled(ARTSCharacter* Character, AController* CharacterOwner);

	/** Event when a player has been defeated. */
	virtual void NotifyOnPlayerDefeated(APlayerController* Player);

	/** Event when a player has been defeated. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS", meta = (DisplayName = "OnPlayerDefeated"))
	void ReceiveOnPlayerDefeated(APlayerController* Player);
};
