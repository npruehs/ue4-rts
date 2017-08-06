#include "RTSPluginPrivatePCH.h"
#include "RTSBuilderComponent.h"

#include "Kismet/GameplayStatics.h"

#include "RTSConstructionSiteComponent.h"
#include "RTSGameMode.h"
#include "RTSPlayerController.h"


void URTSBuilderComponent::AssignToConstructionSite(AActor* ConstructionSite)
{
	if (!ConstructionSite)
	{
		return;
	}

	if (AssignedConstructionSite == ConstructionSite)
	{
		return;
	}

	auto ConstructionSiteComponent = ConstructionSite->FindComponentByClass<URTSConstructionSiteComponent>();

	if (!ConstructionSiteComponent)
	{
		return;
	}

	if (ConstructionSiteComponent->bRequiresBuilderAttention)
	{
		// Assign builder.
		AssignedConstructionSite = ConstructionSite;
		ConstructionSiteComponent->AssignedBuilders.Add(GetOwner());
	}
}

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

	if (!Building)
	{
		return;
	}

	AssignToConstructionSite(Building);
}

void URTSBuilderComponent::BeginConstructionByIndex(int32 BuildingIndex, const FVector& TargetLocation)
{
	if (BuildingIndex < 0 || BuildingIndex >= ConstructibleBuildingTypes.Num())
	{
		return;
	}

	BeginConstruction(ConstructibleBuildingTypes[BuildingIndex], TargetLocation);
}

AActor* URTSBuilderComponent::GetAssignedConstructionSite()
{
	return AssignedConstructionSite;
}

void URTSBuilderComponent::LeaveConstructionSite()
{
	if (!AssignedConstructionSite)
	{
		return;
	}

	auto ConstructionSiteComponent = AssignedConstructionSite->FindComponentByClass<URTSConstructionSiteComponent>();

	if (!ConstructionSiteComponent)
	{
		return;
	}

	// Remove builder.
	AssignedConstructionSite = nullptr;
	ConstructionSiteComponent->AssignedBuilders.Remove(GetOwner());
}
