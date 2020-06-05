#include "Construction/RTSBuilderComponent.h"

#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"

#include "RTSPawnAIController.h"
#include "RTSContainerComponent.h"
#include "RTSGameMode.h"
#include "RTSLog.h"
#include "Construction/RTSConstructionSiteComponent.h"
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
		ConstructionSiteComponent->AssignBuilder(GetOwner());

		// Notify listeners.
		OnAssignedToConstructionSite.Broadcast(GetOwner(), ConstructionSite);

		UE_LOG(LogRTS, Log, TEXT("Builder %s assigned to construction site %s."), *GetOwner()->GetName(), *ConstructionSite->GetName());

		if (bEnterConstructionSite)
		{
			// Enter construction site.
			auto ContainerComponent = ConstructionSite->FindComponentByClass<URTSContainerComponent>();

			if (ContainerComponent)
			{
				ContainerComponent->LoadActor(GetOwner());
				OnConstructionSiteEntered.Broadcast(GetOwner(), ConstructionSite);
			}
		}
	}
}

bool URTSBuilderComponent::BeginConstruction(TSubclassOf<AActor> BuildingClass, const FVector& TargetLocation)
{
	// Get game, pawn and controller.
	ARTSGameMode* GameMode = Cast<ARTSGameMode>(UGameplayStatics::GetGameMode(this));

	if (!GameMode)
	{
		return false;
	}

    auto Pawn = Cast<APawn>(GetOwner());

    if (!Pawn)
    {
        return false;
    }

    auto PawnController = Cast<ARTSPawnAIController>(Pawn->GetController());

    if (!PawnController)
    {
        return false;
    }

    if (BuildingClass == nullptr)
    {
        UE_LOG(LogRTS, Error, TEXT("Builder %s wants to build, but no building class was specified."), *GetOwner()->GetName());
        return false;
    }

    // Check requirements.
    TSubclassOf<AActor> MissingRequirement;

    if (URTSGameplayLibrary::GetMissingRequirementFor(this, GetOwner(), BuildingClass, MissingRequirement))
    {
        UE_LOG(LogRTS, Error, TEXT("Builder %s wants to build %s, but is missing requirement %s."), *GetOwner()->GetName(), *BuildingClass->GetName(), *MissingRequirement->GetName());

        NotifyOnConstructionFailed(Pawn, BuildingClass, Pawn->GetActorLocation());

        // Player is missing a required actor. Stop.
        PawnController->IssueStopOrder();
        return false;
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
        NotifyOnConstructionFailed(Pawn, BuildingClass, Pawn->GetActorLocation());
		return false;
	}

	// Notify listeners.
	OnConstructionStarted.Broadcast(Pawn, Building);

	UE_LOG(LogRTS, Log, TEXT("Builder %s has created construction site %s."), *GetOwner()->GetName(), *Building->GetName());

	// Issue construction order.
	PawnController->IssueContinueConstructionOrder(Building);

    return true;
}

bool URTSBuilderComponent::BeginConstructionByIndex(int32 BuildingIndex, const FVector& TargetLocation)
{
	if (BuildingIndex < 0 || BuildingIndex >= ConstructibleBuildingClasses.Num())
	{
		return false;
	}

	return BeginConstruction(ConstructibleBuildingClasses[BuildingIndex], TargetLocation);
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
	ConstructionSiteComponent->UnassignBuilder(GetOwner());

	// Notify listeners.
	OnRemovedFromConstructionSite.Broadcast(GetOwner(), ConstructionSite);

	UE_LOG(LogRTS, Log, TEXT("Builder %s has been unassigned from construction site %s."), *GetOwner()->GetName(), *ConstructionSite->GetName());

	if (bEnterConstructionSite)
	{
		// Leave construction site.
		auto ContainerComponent = ConstructionSite->FindComponentByClass<URTSContainerComponent>();

		if (ContainerComponent)
		{
			ContainerComponent->UnloadActor(GetOwner());
			OnConstructionSiteLeft.Broadcast(GetOwner(), ConstructionSite);
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

void URTSBuilderComponent::NotifyOnConstructionFailed(AActor* Builder, TSubclassOf<AActor> BuildingClass, const FVector& Location)
{
    OnConstructionFailed.Broadcast(Builder, BuildingClass, Location);
}
