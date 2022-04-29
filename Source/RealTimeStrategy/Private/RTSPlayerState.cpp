#include "RTSPlayerState.h"

#include "EngineUtils.h"
#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"

#include "RTSLog.h"
#include "RTSOwnerComponent.h"
#include "RTSPlayerController.h"
#include "RTSTeamInfo.h"


const uint8 ARTSPlayerState::PLAYER_INDEX_NONE = 255;


ARTSPlayerState::ARTSPlayerState(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
	PlayerIndex = PLAYER_INDEX_NONE;
}

void ARTSPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARTSPlayerState, PlayerIndex);
	DOREPLIFETIME(ARTSPlayerState, Team);
}

uint8 ARTSPlayerState::GetPlayerIndex() const
{
	return PlayerIndex;
}

ARTSTeamInfo* ARTSPlayerState::GetTeam() const
{
	return Team;
}

void ARTSPlayerState::SetPlayerIndex(uint8 InPlayerIndex)
{
	PlayerIndex = InPlayerIndex;
}

void ARTSPlayerState::SetTeam(ARTSTeamInfo* InTeam)
{
	Team = InTeam;
}

bool ARTSPlayerState::IsSameTeamAs(const ARTSPlayerState* Other) const
{
	if (!Other)
	{
		return false;
	}

	const ARTSTeamInfo* FirstTeam = Team;
	const ARTSTeamInfo* SecondTeam = Other->Team;

	if (!FirstTeam || !SecondTeam)
	{
		return false;
	}

	return FirstTeam->GetTeamIndex() == SecondTeam->GetTeamIndex();
}

TArray<AActor*> ARTSPlayerState::GetOwnActors() const
{
	return OwnActors;
}

void ARTSPlayerState::DiscoverOwnActors()
{
	OwnActors.Empty();

	for (TActorIterator<AActor> ActorIt(GetWorld()); ActorIt; ++ActorIt)
	{
		AActor* Actor = *ActorIt;

		if (!IsValid(Actor))
		{
			continue;
		}

		const URTSOwnerComponent* OwnerComponent = Actor->FindComponentByClass<URTSOwnerComponent>();

		if (IsValid(OwnerComponent) && OwnerComponent->GetPlayerOwner() == this)
		{
			OwnActors.AddUnique(Actor);
		}
	}
}

void ARTSPlayerState::NotifyOnTeamChanged(ARTSTeamInfo* NewTeam)
{
	if (NewTeam)
	{
		UE_LOG(LogRTS, Log, TEXT("Player %s added to team %d."), *GetName(), NewTeam->GetTeamIndex());
	}
	else
	{
		UE_LOG(LogRTS, Log, TEXT("Player %s added to team None."), *GetName());
	}

	// Notify listeners.
	ReceiveOnTeamChanged(NewTeam);

	// Notify player.
	ARTSPlayerController* PlayerController = Cast<ARTSPlayerController>(GetOwner());

	if (PlayerController)
	{
		PlayerController->NotifyOnTeamChanged(NewTeam);
	}
}

void ARTSPlayerState::NotifyOnActorOwnerChanged(AActor* Actor, ARTSPlayerState* OldOwner, ARTSPlayerState* NewOwner)
{
	// Update list of own actors.
	if (NewOwner == this)
	{
		OwnActors.AddUnique(Actor);
	}
	else
	{
		OwnActors.Remove(Actor);
	}

	// Notify listeners.
	ARTSPlayerController* PlayerController = Cast<ARTSPlayerController>(GetOwner());

	if (IsValid(PlayerController))
	{
		PlayerController->NotifyOnActorOwnerChanged(Actor);
	}
}

void ARTSPlayerState::OnTeamChanged()
{
	NotifyOnTeamChanged(Team);
}
