#include "RTSGameState.h"

#include "Engine/World.h"

#include "Vision/RTSVisionManager.h"


void ARTSGameState::BeginPlay()
{
	// Make sure all actors have begun play.
	Super::BeginPlay();

	// Setup vision.
	VisionManager = GetWorld()->SpawnActor<ARTSVisionManager>();
	VisionManager->Initialize();
}

ARTSVisionManager* ARTSGameState::GetVisionManager() const
{
	return VisionManager;
}
