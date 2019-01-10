#include "RTSCheatManager.h"

#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"

#include "RTSGameMode.h"
#include "RTSLog.h"
#include "RTSPlayerController.h"
#include "RTSPlayerAIController.h"
#include "RTSPlayerAdvantageComponent.h"
#include "Economy/RTSPlayerResourcesComponent.h"


void URTSCheatManager::Boost()
{
    APlayerController* Player = GetOuterAPlayerController();

    if (!Player)
    {
        return;
    }

    URTSPlayerAdvantageComponent* PlayerAdvantageComponent = Player->FindComponentByClass<URTSPlayerAdvantageComponent>();

    if (!PlayerAdvantageComponent)
    {
        return;
    }

    PlayerAdvantageComponent->SetSpeedBoostFactor(PlayerAdvantageComponent->GetSpeedBoostFactor() * SpeedBoostFactor);
    UE_LOG(LogRTS, Log, TEXT("Cheat: Set speed boost factor to %f."), PlayerAdvantageComponent->GetSpeedBoostFactor());
}

void URTSCheatManager::God()
{
    Super::God();

    APlayerController* Player = GetOuterAPlayerController();

    if (!Player)
    {
        return;
    }

    URTSPlayerAdvantageComponent* PlayerAdvantageComponent = Player->FindComponentByClass<URTSPlayerAdvantageComponent>();

    if (!PlayerAdvantageComponent)
    {
        return;
    }

    // Toggle god mode.
    PlayerAdvantageComponent->SetGodModeEnabled(!PlayerAdvantageComponent->IsGodModeEnabled());

    for (TActorIterator<APawn> PawnItr(GetWorld()); PawnItr; ++PawnItr)
    {
        APawn* Pawn = *PawnItr;

        if (!IsValid(Pawn) || Pawn->GetOwner() != Player)
        {
            continue;
        }

        Pawn->bCanBeDamaged = !PlayerAdvantageComponent->IsGodModeEnabled();
    }
}

void URTSCheatManager::Money()
{
    APlayerController* Player = GetOuterAPlayerController();

    if (!Player)
    {
        return;
    }

    URTSPlayerResourcesComponent* PlayerResourcesComponent = Player->FindComponentByClass<URTSPlayerResourcesComponent>();

    if (!PlayerResourcesComponent)
    {
        return;
    }

    for (TSubclassOf<URTSResourceType> ResourceType : ResourceTypes)
    {
        PlayerResourcesComponent->AddResources(ResourceType, ResourceAmount);
        UE_LOG(LogRTS, Log, TEXT("Cheat: Added %f %s."), ResourceAmount, *ResourceType->GetName());
    }
}

void URTSCheatManager::Victory()
{
    APlayerController* Player = GetOuterAPlayerController();

    if (!Player)
    {
        return;
    }

    ARTSGameMode* GameMode = Cast<ARTSGameMode>(UGameplayStatics::GetGameMode(Player));

    if (!GameMode)
    {
        return;
    }

    for (TActorIterator<AController> ControllerItr(GetWorld()); ControllerItr; ++ControllerItr)
    {
        AController* Controller = *ControllerItr;

        if (!IsValid(Controller) || Controller == Player)
        {
            continue;
        }

        if (Cast<ARTSPlayerController>(Controller) == nullptr &&
            Cast <ARTSPlayerAIController>(Controller) == nullptr)
        {
            continue;
        }

        GameMode->NotifyOnPlayerDefeated(Controller);
    }
}
