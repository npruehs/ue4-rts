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
#include "Vision/RTSVisionInfo.h"


URTSCheatManager::URTSCheatManager(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
{
	// Set reasonable default values.
	OutgoingDamageFactor = 1000.0f;
	ResourceAmount = 1000.0f;
	SpeedBoostFactor = 10.0f;
}

void URTSCheatManager::Boost()
{
	const APlayerController* Player = GetOuterAPlayerController();

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

void URTSCheatManager::Damage()
{
	const APlayerController* Player = GetOuterAPlayerController();

	if (!Player)
	{
		return;
	}

	URTSPlayerAdvantageComponent* PlayerAdvantageComponent = Player->FindComponentByClass<URTSPlayerAdvantageComponent>();

	if (!PlayerAdvantageComponent)
	{
		return;
	}

	if (PlayerAdvantageComponent->GetOutgoingDamageFactor() > 1.0f)
	{
		PlayerAdvantageComponent->SetOutgoingDamageFactor(1.0f);
	}

	else
	{
		PlayerAdvantageComponent->SetOutgoingDamageFactor(OutgoingDamageFactor);
	}

	UE_LOG(LogRTS, Log, TEXT("Cheat: Set outgoing damage factor to %f."), PlayerAdvantageComponent->GetOutgoingDamageFactor());
}

void URTSCheatManager::God()
{
	Super::God();

	const APlayerController* Player = GetOuterAPlayerController();

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

		Pawn->SetCanBeDamaged(!PlayerAdvantageComponent->IsGodModeEnabled());
	}
}

void URTSCheatManager::Money()
{
	const APlayerController* Player = GetOuterAPlayerController();

	if (!Player)
	{
		return;
	}

	URTSPlayerResourcesComponent* PlayerResourcesComponent = Player->FindComponentByClass<URTSPlayerResourcesComponent>();

	if (!PlayerResourcesComponent)
	{
		return;
	}

	for (const TSubclassOf<URTSResourceType> ResourceType : ResourceTypes)
	{
		PlayerResourcesComponent->AddResources(ResourceType, ResourceAmount);
		UE_LOG(LogRTS, Log, TEXT("Cheat: Added %f %s."), ResourceAmount, *ResourceType->GetName());
	}
}

void URTSCheatManager::NoFog()
{
	for (TActorIterator<ARTSVisionInfo> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ARTSVisionInfo* VisionInfo = *ActorItr;
		VisionInfo->SetRevealed(!VisionInfo->IsRevealed());

		UE_LOG(LogRTS, Log, TEXT("Cheat: Set vision to %s."), VisionInfo->IsRevealed() ? TEXT("revealed") : TEXT("not revealed"));
	}
}

void URTSCheatManager::Victory()
{
	const APlayerController* Player = GetOuterAPlayerController();

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
			Cast<ARTSPlayerAIController>(Controller) == nullptr)
		{
			continue;
		}

		GameMode->NotifyOnPlayerDefeated(Controller);
	}
}

void URTSCheatManager::Weak()
{
	const APlayerController* Player = GetOuterAPlayerController();

	if (!Player)
	{
		return;
	}

	URTSPlayerAdvantageComponent* PlayerAdvantageComponent = Player->FindComponentByClass<URTSPlayerAdvantageComponent>();

	if (!PlayerAdvantageComponent)
	{
		return;
	}

	if (PlayerAdvantageComponent->GetOutgoingDamageFactor() < 1.0f)
	{
		PlayerAdvantageComponent->SetOutgoingDamageFactor(1.0f);
	}

	else
	{
		PlayerAdvantageComponent->SetOutgoingDamageFactor(1.0f / OutgoingDamageFactor);
	}

	UE_LOG(LogRTS, Log, TEXT("Cheat: Set outgoing damage factor to %f."), PlayerAdvantageComponent->GetOutgoingDamageFactor());
}
