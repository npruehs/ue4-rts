#include "RTSPluginPrivatePCH.h"
#include "RTSBuilderComponent.h"

#include "Kismet/GameplayStatics.h"

#include "RTSGameMode.h"
#include "RTSPlayerController.h"


void URTSBuilderComponent::BeginConstruction(TSubclassOf<AActor> BuildingType, const FVector& TargetLocation)
{
	// Get game.
	UWorld* World = GetWorld();

	if (!World)
	{
		return;
	}

	ARTSGameMode* GameMode = Cast<ARTSGameMode>(UGameplayStatics::GetGameMode(World));

	if (!GameMode)
	{
		return;
	}

	// Spawn building.
	AActor* Building = GameMode->SpawnActorForPlayer(
		BuildingType,
		Cast<ARTSPlayerController>(GetOwner()->GetOwner()),
		FTransform(FRotator::ZeroRotator, TargetLocation));
}

void URTSBuilderComponent::BeginConstructionByIndex(int32 BuildingIndex, const FVector& TargetLocation)
{
	if (BuildingIndex < 0 || BuildingIndex >= ConstructibleBuildingTypes.Num())
	{
		return;
	}

	BeginConstruction(ConstructibleBuildingTypes[BuildingIndex], TargetLocation);
}
