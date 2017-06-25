#include "RTSPluginPrivatePCH.h"
#include "RTSGameMode.h"

#include "Engine/World.h"
#include "GameFramework/GameModeBase.h"

#include "RTSCharacter.h"
#include "RTSPlayerController.h"


void ARTSGameMode::RestartPlayerAtPlayerStart(AController* NewPlayer, AActor* StartSpot)
{
	// Spawn default pawns (most likely, this will be the camera pawn in our case).
	AGameModeBase::RestartPlayerAtPlayerStart(NewPlayer, StartSpot);

	// Verify parameters.
	if (NewPlayer == nullptr || NewPlayer->IsPendingKillPending())
	{
		return;
	}

	if (!StartSpot)
	{
		return;
	}

	for (TSubclassOf<AActor> ActorClass : InitialActors)
	{
		// Build spawn transform.
		// Don't allow initial actors to be spawned with any pitch or roll.
		FVector SpawnLocation = StartSpot->GetActorLocation();

		FRotator SpawnRotation(ForceInit);
		SpawnRotation.Yaw = StartSpot->GetActorRotation().Yaw;

		FTransform SpawnTransform = FTransform(SpawnRotation, SpawnLocation);

		// Build spawn info.
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.Instigator = Instigator;
		SpawnInfo.ObjectFlags |= RF_Transient;

		// Spawn actor.
		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ActorClass, SpawnTransform, SpawnInfo);
		ARTSPlayerController* PlayerController = Cast<ARTSPlayerController>(NewPlayer);

		if (SpawnedActor && PlayerController)
		{
			UE_LOG(RTSLog, Log, TEXT("Spawned %s for player %s at %s."), *SpawnedActor->GetName(), *NewPlayer->GetName(), *SpawnLocation.ToString());

			// Set owning player.
			PlayerController->TransferOwnership(SpawnedActor);
		}
	}
}

void ARTSGameMode::NotifyOnCharacterKilled(ARTSCharacter* Character, AController* Owner)
{
	if (DefeatConditionActor == nullptr)
	{
		return;
	}

	APlayerController* OwningPlayer = Cast<APlayerController>(Owner);

	if (OwningPlayer == nullptr)
	{
		return;
	}

	// Check if any required actors are still alive.
	for (AActor* OwnedActor : OwningPlayer->Children)
	{
		if (OwnedActor->GetClass() == DefeatConditionActor)
		{
			return;
		}
	}

	UE_LOG(RTSLog, Log, TEXT("Player %s does not control any %s anymore and has been defeated."), *OwningPlayer->GetName(), *DefeatConditionActor->GetName());

	// Notify listeners.
	NotifyOnPlayerDefeated(OwningPlayer);
}

void ARTSGameMode::NotifyOnPlayerDefeated(APlayerController* Player)
{
	ReceiveOnPlayerDefeated(Player);
}