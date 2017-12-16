#include "RTSPluginPCH.h"
#include "RTSBuilderComponent.h"

#include "Kismet/GameplayStatics.h"

#include "RTSCharacterAIController.h"
#include "RTSConstructionSiteComponent.h"
#include "RTSContainerComponent.h"
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

	if (ConstructionSiteComponent->CanAssignBuilder(GetOwner()))
	{
		// Assign builder.
		AssignedConstructionSite = ConstructionSite;
		ConstructionSiteComponent->AssignedBuilders.Add(GetOwner());

		// Notify listeners.
		OnAssignedToConstructionSite.Broadcast(ConstructionSite);

		UE_LOG(LogRTS, Log, TEXT("Builder %s assigned to construction site %s."), *GetOwner()->GetName(), *ConstructionSite->GetName());

		if (bEnterConstructionSite)
		{
			// Enter construction site.
			auto ContainerComponent = ConstructionSite->FindComponentByClass<URTSContainerComponent>();

			if (ContainerComponent)
			{
				ContainerComponent->LoadActor(GetOwner());
				OnConstructionSiteEntered.Broadcast(ConstructionSite);
			}
		}
	}
}

void URTSBuilderComponent::BeginConstruction(TSubclassOf<AActor> BuildingClass, const FVector& TargetLocation)
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
		BuildingClass,
		Cast<ARTSPlayerController>(GetOwner()->GetOwner()),
		FTransform(FRotator::ZeroRotator, TargetLocation));

	if (!Building)
	{
		return;
	}

	// Notify listeners.
	OnConstructionStarted.Broadcast(Building);

	UE_LOG(LogRTS, Log, TEXT("Builder %s has created construction site %s."), *GetOwner()->GetName(), *Building->GetName());

	// Issue construction order.
	auto Pawn = Cast<APawn>(GetOwner());

	if (!Pawn)
	{
		return;
	}

	auto PawnController = Cast<ARTSCharacterAIController>(Pawn->GetController());

	if (!PawnController)
	{
		return;
	}

	PawnController->IssueContinueConstructionOrder(Building);
}

void URTSBuilderComponent::BeginConstructionByIndex(int32 BuildingIndex, const FVector& TargetLocation)
{
	if (BuildingIndex < 0 || BuildingIndex >= ConstructibleBuildingClasses.Num())
	{
		return;
	}

	BeginConstruction(ConstructibleBuildingClasses[BuildingIndex], TargetLocation);
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

	auto ConstructionSite = AssignedConstructionSite;
	auto ConstructionSiteComponent = ConstructionSite->FindComponentByClass<URTSConstructionSiteComponent>();

	if (!ConstructionSiteComponent)
	{
		return;
	}

	// Remove builder.
	AssignedConstructionSite = nullptr;
	ConstructionSiteComponent->AssignedBuilders.Remove(GetOwner());

	// Notify listeners.
	OnRemovedFromConstructionSite.Broadcast(ConstructionSite);

	UE_LOG(LogRTS, Log, TEXT("Builder %s has been unassigned from construction site %s."), *GetOwner()->GetName(), *ConstructionSite->GetName());

	if (bEnterConstructionSite)
	{
		// Leave construction site.
		auto ContainerComponent = ConstructionSite->FindComponentByClass<URTSContainerComponent>();

		if (ContainerComponent)
		{
			ContainerComponent->UnloadActor(GetOwner());
			OnConstructionSiteLeft.Broadcast(ConstructionSite);
		}
	}
}
