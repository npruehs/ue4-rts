#include "RTSPluginPrivatePCH.h"
#include "RTSGameMode.h"

#include "Engine/World.h"
#include "GameFramework/GameModeBase.h"


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

		if (SpawnedActor)
		{
			// Set owning player.
			SpawnedActor->SetOwner(NewPlayer);

			UE_LOG(RTSLog, Log, TEXT("Spawned %s for player %s at %s."), *SpawnedActor->GetName(), *NewPlayer->GetName(), *SpawnLocation.ToString());
		}
	}
}
