#include "RTSPluginPCH.h"
#include "RTSGameMode.h"

#include "CString.h"
#include "EngineUtils.h"
#include "Engine/World.h"
#include "GameFramework/Controller.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"

#include "RTSCharacter.h"
#include "RTSConstructionSiteComponent.h"
#include "RTSOwnerComponent.h"
#include "RTSPlayerAIController.h"
#include "RTSPlayerAdvantageComponent.h"
#include "RTSPlayerController.h"
#include "RTSPlayerStart.h"
#include "RTSTeamInfo.h"
#include "RTSVisionInfo.h"


ARTSGameMode::ARTSGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set reasonable default values.
	NumTeams = 2;
}

void ARTSGameMode::BeginPlay()
{
    Super::BeginPlay();

    // Parse options.
    FString NumAIPlayersString = UGameplayStatics::ParseOption(OptionsString, TEXT("NumAIPlayers"));
    NumAIPlayers = FCString::Atoi(*NumAIPlayersString);

    UE_LOG(LogRTS, Log, TEXT("NumAIPlayers = %i"), NumAIPlayers);

    // Spawn AI players.
    for (int32 Index = 0; Index < NumAIPlayers; ++Index)
    {
        ARTSPlayerAIController* NewAI = StartAIPlayer();

        if (NewAI != nullptr)
        {
            NewAI->PlayerState->SetPlayerName(FString::Printf(TEXT("AI Player %i"), Index + 1));
        }
    }
}

void ARTSGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	AGameModeBase::InitGame(MapName, Options, ErrorMessage);

	// Set up teams.
	if (TeamClass == nullptr)
	{
		TeamClass = ARTSTeamInfo::StaticClass();
	}

	for (uint8 TeamIndex = 0; TeamIndex < NumTeams; ++TeamIndex)
	{
		// Add team.
		ARTSTeamInfo* NewTeam = GetWorld()->SpawnActor<ARTSTeamInfo>(TeamClass);
		NewTeam->TeamIndex = TeamIndex;
		Teams.Add(NewTeam);

		// Setup vision.
		ARTSVisionInfo* TeamVision = GetWorld()->SpawnActor<ARTSVisionInfo>();
		TeamVision->SetTeamIndex(TeamIndex);
	}
}

ARTSPlayerStart* ARTSGameMode::FindRTSPlayerStart(AController* Player)
{
	// Choose a player start.
	TArray<ARTSPlayerStart*> UnOccupiedStartPoints;
	TArray<ARTSPlayerStart*> OccupiedStartPoints;

	for (TActorIterator<ARTSPlayerStart> It(GetWorld()); It; ++It)
	{
		ARTSPlayerStart* PlayerStart = *It;

		if (PlayerStart->Player == nullptr)
		{
			UnOccupiedStartPoints.Add(PlayerStart);
		}
		else
		{
			OccupiedStartPoints.Add(PlayerStart);
		}
	}

	if (UnOccupiedStartPoints.Num() > 0)
	{
		return UnOccupiedStartPoints[FMath::RandRange(0, UnOccupiedStartPoints.Num() - 1)];
	}
	else if (OccupiedStartPoints.Num() > 0)
	{
		return OccupiedStartPoints[FMath::RandRange(0, OccupiedStartPoints.Num() - 1)];
	}

	return nullptr;
}

void ARTSGameMode::RestartPlayer(AController* NewPlayer)
{
	if (NewPlayer == nullptr || NewPlayer->IsPendingKillPending())
	{
		return;
	}

	ARTSPlayerStart* StartSpot = FindRTSPlayerStart(NewPlayer);
	RestartPlayerAtPlayerStart(NewPlayer, StartSpot);
}

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

	// Occupy start spot.
	ARTSPlayerStart* PlayerStart = Cast<ARTSPlayerStart>(StartSpot);

	if (PlayerStart)
	{
		UE_LOG(LogRTS, Log, TEXT("Start spot %s is now occupied by player %s."), *PlayerStart->GetName(), *NewPlayer->GetName());
		PlayerStart->Player = NewPlayer;
	}

	// Set team.
	if (PlayerStart->TeamIndex >= Teams.Num())
	{
		UE_LOG(LogRTS, Warning, TEXT("Player start team index is %d, but game only has %d teams."), PlayerStart->TeamIndex, Teams.Num());
	}
	else
	{
		Teams[PlayerStart->TeamIndex]->AddToTeam(NewPlayer);
	}

	// Build spawn transform.
	// Don't allow initial actors to be spawned with any pitch or roll.
	FRotator SpawnRotation(ForceInit);
	SpawnRotation.Yaw = StartSpot->GetActorRotation().Yaw;

	// Build spawn info.
	for (int32 i = 0; i < InitialActors.Num(); ++i)
	{
        TSubclassOf<AActor> ActorClass = InitialActors[i];

		// Spawn actor.
        FVector SpawnLocation = StartSpot->GetActorLocation();

        if (i < InitialActorPositions.Num())
        {
            SpawnLocation += InitialActorPositions[i];
        }
        
        FTransform SpawnTransform = FTransform(SpawnRotation, SpawnLocation);
		AActor* SpawnedActor = SpawnActorForPlayer(ActorClass, NewPlayer, SpawnTransform);

        // Finish construction of initial buildings immediately.
        if (SpawnedActor != nullptr)
        {
            URTSConstructionSiteComponent* ConstructionSiteComponent = SpawnedActor->FindComponentByClass<URTSConstructionSiteComponent>();

            if (ConstructionSiteComponent != nullptr)
            {
                ConstructionSiteComponent->FinishConstruction();
            }
        }
	}
}

ARTSPlayerAIController* ARTSGameMode::StartAIPlayer()
{
    FActorSpawnParameters SpawnInfo;
    SpawnInfo.Instigator = Instigator;
    SpawnInfo.ObjectFlags |= RF_Transient;	// We never want to save player controllers into a map
    SpawnInfo.bDeferConstruction = true;
    ARTSPlayerAIController* NewAI = GetWorld()->SpawnActor<ARTSPlayerAIController>(PlayerAIControllerClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnInfo);
    if (NewAI)
    {
        UGameplayStatics::FinishSpawningActor(NewAI, FTransform(FRotator::ZeroRotator, FVector::ZeroVector));

        UE_LOG(LogRTS, Log, TEXT("Spawned AI player %s."), *NewAI->GetName());
    }
    else
    {
        UE_LOG(LogRTS, Error, TEXT("Failed to spawn AI player."));
        return nullptr;
    }

    RestartPlayer(NewAI);
    return NewAI;
}

AActor* ARTSGameMode::SpawnActorForPlayer(TSubclassOf<AActor> ActorClass, AController* ActorOwner, const FTransform& SpawnTransform)
{
	// Spawn actor.
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ActorClass->GetDefaultObject()->GetClass(), SpawnTransform, SpawnParams);

	// Set owning player.
	if (SpawnedActor && ActorOwner)
	{
		UE_LOG(LogRTS, Log, TEXT("Spawned %s for player %s at %s."), *SpawnedActor->GetName(), *ActorOwner->GetName(), *SpawnTransform.GetLocation().ToString());

		// Set owning player.
		TransferOwnership(SpawnedActor, ActorOwner);
	}

	return SpawnedActor;
}

void ARTSGameMode::TransferOwnership(AActor* Actor, AController* NewOwner)
{
    if (!Actor || !NewOwner)
    {
        return;
    }

    // Set owning player.
    Actor->SetOwner(NewOwner);

    URTSOwnerComponent* OwnerComponent = Actor->FindComponentByClass<URTSOwnerComponent>();

    if (OwnerComponent)
    {
        OwnerComponent->SetPlayerOwner(NewOwner);
    }

    UE_LOG(LogRTS, Log, TEXT("Player %s is now owning %s."), *NewOwner->GetName(), *Actor->GetName());

    // Check for god mode.
    URTSPlayerAdvantageComponent* PlayerAdvantageComponent = NewOwner->FindComponentByClass<URTSPlayerAdvantageComponent>();

    if (PlayerAdvantageComponent)
    {
        APawn* Pawn = Cast<APawn>(Actor);

        if (Pawn)
        {
            Pawn->bCanBeDamaged = !PlayerAdvantageComponent->bGodModeEnabled;
        }
    }

    // Notify listeners.
    ARTSPlayerController* NewPlayerOwner = Cast<ARTSPlayerController>(NewOwner);
    
    if (NewPlayerOwner != nullptr)
    {
        NewPlayerOwner->NotifyOnActorOwnerChanged(Actor);
    }    
}

void ARTSGameMode::NotifyOnActorKilled(AActor* Actor, AController* ActorOwner)
{
	if (DefeatConditionActor == nullptr)
	{
		return;
	}

	ARTSPlayerController* OwningPlayer = Cast<ARTSPlayerController>(ActorOwner);

	if (OwningPlayer == nullptr)
	{
        ARTSPlayerAIController* OwningAIPlayer = Cast<ARTSPlayerAIController>(ActorOwner);

        if (OwningAIPlayer == nullptr)
        {
            return;
        }
	}

	// Check if any required actors are still alive.
	for (AActor* OwnedActor : ActorOwner->Children)
	{
		if (OwnedActor->GetClass() == DefeatConditionActor)
		{
			return;
		}
	}

	UE_LOG(LogRTS, Log, TEXT("Player %s does not control any %s anymore and has been defeated."), *ActorOwner->GetName(), *DefeatConditionActor->GetName());

	// Notify listeners.
	NotifyOnPlayerDefeated(ActorOwner);
}

void ARTSGameMode::NotifyOnPlayerDefeated(AController* Player)
{
	ReceiveOnPlayerDefeated(Player);
}
