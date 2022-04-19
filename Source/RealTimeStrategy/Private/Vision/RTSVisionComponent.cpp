#include "Vision/RTSVisionComponent.h"

#include "Kismet/GameplayStatics.h"

#include "RTSGameState.h"
#include "Vision/RTSVisionManager.h"


URTSVisionComponent::URTSVisionComponent(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
	// Set reasonable default values.
	SightRadius = 1000.0f;
}

void URTSVisionComponent::BeginPlay()
{
	Super::BeginPlay();

	// Register actor with game.
	const ARTSGameState* GameState = Cast<ARTSGameState>(UGameplayStatics::GetGameState(this));

	if (IsValid(GameState))
	{
		ARTSVisionManager* VisionManager = GameState->GetVisionManager();

		if (IsValid(VisionManager))
		{
			VisionManager->AddVisionActor(GetOwner());
		}
	}
}

void URTSVisionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (EndPlayReason == EEndPlayReason::Destroyed)
	{
		// Unregister actor from game.
		const ARTSGameState* GameState = Cast<ARTSGameState>(UGameplayStatics::GetGameState(this));

		if (IsValid(GameState))
		{
			ARTSVisionManager* VisionManager = GameState->GetVisionManager();

			if (IsValid(VisionManager))
			{
				VisionManager->RemoveVisionActor(GetOwner());
			}
		}
	}
}

float URTSVisionComponent::GetSightRadius() const
{
	return SightRadius;
}

bool URTSVisionComponent::IgnoresHeightLevels() const
{
	return bIgnoreHeightLevels;
}
