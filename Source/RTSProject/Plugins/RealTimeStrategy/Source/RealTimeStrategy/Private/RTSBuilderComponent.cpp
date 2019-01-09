#include "RealTimeStrategyPCH.h"
#include "RTSBuilderComponent.h"

#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"

#include "RTSCharacterAIController.h"
#include "RTSConstructionSiteComponent.h"
#include "RTSContainerComponent.h"
#include "RTSGameMode.h"
#include "Libraries/RTSCollisionLibrary.h"
#include "Libraries/RTSGameplayLibrary.h"


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
		ConstructionSiteComponent->GetAssignedBuilders().Add(GetOwner());

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
	// Get game, pawn and controller.
	ARTSGameMode* GameMode = Cast<ARTSGameMode>(UGameplayStatics::GetGameMode(this));

	if (!GameMode)
	{
		return;
	}

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

    // Check requirements.
    TSubclassOf<AActor> MissingRequirement;

    if (URTSGameplayLibrary::GetMissingRequirementFor(this, GetOwner(), BuildingClass, MissingRequirement))
    {
        UE_LOG(LogRTS, Error, TEXT("Builder %s wants to build %s, but is missing requirement %s."), *GetOwner()->GetName(), *BuildingClass->GetName(), *MissingRequirement->GetName());

        // Player is missing a required actor. Stop.
        PawnController->IssueStopOrder();
        return;
    }

    // Move builder away in order to avoid collision.
    FVector BuilderLocation = Pawn->GetActorLocation();
    FVector ToTargetLocation = TargetLocation - BuilderLocation;
    ToTargetLocation.Z = 0.0f;
    FVector ToTargetLocationNormalized = ToTargetLocation.GetSafeNormal();
    float SafetyDistance = 
        (URTSCollisionLibrary::GetActorCollisionSize(Pawn) / 2 +
         URTSCollisionLibrary::GetCollisionSize(BuildingClass) / 2)
        + ConstructionSiteOffset;

    FVector SafeBuilderLocation = TargetLocation - ToTargetLocationNormalized * SafetyDistance;
    SafeBuilderLocation.Z = BuilderLocation.Z;

    Pawn->SetActorLocation(SafeBuilderLocation);

	// Spawn building.
	AActor* Building = GameMode->SpawnActorForPlayer(
		BuildingClass,
		Cast<AController>(GetOwner()->GetOwner()),
		FTransform(FRotator::ZeroRotator, TargetLocation));

	if (!Building)
	{
		return;
	}

	// Notify listeners.
	OnConstructionStarted.Broadcast(Building);

	UE_LOG(LogRTS, Log, TEXT("Builder %s has created construction site %s."), *GetOwner()->GetName(), *Building->GetName());

	// Issue construction order.
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
	ConstructionSiteComponent->GetAssignedBuilders().Remove(GetOwner());

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

TArray<TSubclassOf<AActor>> URTSBuilderComponent::GetConstructibleBuildingClasses() const
{
    return ConstructibleBuildingClasses;
}

bool URTSBuilderComponent::DoesEnterConstructionSite() const
{
    return bEnterConstructionSite;
}

float URTSBuilderComponent::GetConstructionSiteOffset() const
{
    return ConstructionSiteOffset;
}

AActor* URTSBuilderComponent::GetAssignedConstructionSite() const
{
    return AssignedConstructionSite;
}
