#include "RTSOwnerComponent.h"

#include "Engine/World.h"
#include "GameFramework/Controller.h"
#include "Net/UnrealNetwork.h"

#include "RTSPlayerState.h"
#include "RTSTeamInfo.h"


URTSOwnerComponent::URTSOwnerComponent(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
    : Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(true);

    InitialOwnerPlayerIndex = ARTSPlayerState::PLAYER_INDEX_NONE;
}

void URTSOwnerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URTSOwnerComponent, PlayerOwner);
}


ARTSPlayerState* URTSOwnerComponent::GetPlayerOwner() const
{
	return PlayerOwner;
}

void URTSOwnerComponent::SetPlayerOwner(AController* Controller)
{
    if (!IsValid(Controller))
    {
        SetPlayerStateOwner(nullptr);
    }
    else
    {
        SetPlayerStateOwner(Cast<ARTSPlayerState>(Controller->PlayerState));
    }
}

void URTSOwnerComponent::SetPlayerStateOwner(ARTSPlayerState* PlayerState)
{
    ARTSPlayerState* PreviousOwner = PlayerOwner;

    PlayerOwner = PlayerState;

    if (PlayerOwner != PreviousOwner)
    {
        NotifyOnOwnerChanged(PreviousOwner, PlayerOwner);
    }
}

bool URTSOwnerComponent::IsSameTeamAsActor(AActor* Other) const
{
	if (!Other)
	{
		return false;
	}

	ARTSPlayerState* MyOwner = GetPlayerOwner();

	if (!MyOwner)
	{
		return false;
	}

	URTSOwnerComponent* OtherOwnerComponent = Other->FindComponentByClass<URTSOwnerComponent>();

	if (!OtherOwnerComponent)
	{
		return false;
	}

	ARTSPlayerState* OtherOwner = OtherOwnerComponent->GetPlayerOwner();

	return MyOwner->IsSameTeamAs(OtherOwner);
}

bool URTSOwnerComponent::IsSameTeamAsController(AController* C) const
{
    if (!IsValid(C))
    {
        return false;
    }

	ARTSPlayerState* MyOwner = GetPlayerOwner();
	ARTSPlayerState* OtherPlayer = Cast<ARTSPlayerState>(C->PlayerState);

	return MyOwner && MyOwner->IsSameTeamAs(OtherPlayer);
}

uint8 URTSOwnerComponent::GetInitialOwnerPlayerIndex()
{
    return InitialOwnerPlayerIndex;
}

void URTSOwnerComponent::ReceivedPlayerOwner(ARTSPlayerState* PreviousOwner)
{
    NotifyOnOwnerChanged(PreviousOwner, PlayerOwner);
}

void URTSOwnerComponent::NotifyOnOwnerChanged(ARTSPlayerState* PreviousOwner, ARTSPlayerState* NewOwner)
{
    // Notify listeners.
    OnOwnerChanged.Broadcast(GetOwner(), Cast<AController>(NewOwner->GetOwner()));

    UWorld* World = GetWorld();

    if (IsValid(World))
    {
        for (FConstControllerIterator It = World->GetControllerIterator(); It; ++It)
        {
            TWeakObjectPtr<AController> Controller = *It;
            ARTSPlayerState* PlayerState = Controller->GetPlayerState<ARTSPlayerState>();

            if (IsValid(PlayerState) && (PlayerState == PreviousOwner || PlayerState == NewOwner))
            {
                PlayerState->NotifyOnActorOwnerChanged(GetOwner(), PreviousOwner, NewOwner);
                continue;
            }
        }
    }
}
