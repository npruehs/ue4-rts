#include "RTSSelectableComponent.h"

#include "WorldCollision.h"
#include "Components/DecalComponent.h"
#include "GameFramework/Actor.h"
#include "Libraries/RTSCollisionLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"

#include "RTSLog.h"


void URTSSelectableComponent::BeginPlay()
{
    Super::BeginPlay();

    AActor* Owner = GetOwner();

    if (!IsValid(Owner))
    {
        return;
    }

    // Calculate decal size.
    float DecalHeight = URTSCollisionLibrary::GetActorCollisionHeight(Owner);
    float DecalRadius = URTSCollisionLibrary::GetActorCollisionSize(Owner);

    // Create selection circle decal.
    DecalComponent = NewObject<UDecalComponent>(Owner, TEXT("SelectionCircleDecal"));

    if (!DecalComponent)
    {
        return;
    }

    // Set decal size.
    DecalComponent->RegisterComponent();
    DecalComponent->AttachToComponent(Owner->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
    DecalComponent->DecalSize = FVector(DecalHeight, DecalRadius, DecalRadius);

    // Rotate decal to face ground.
    DecalComponent->SetRelativeRotation(FRotator::MakeFromEuler(FVector(0.0f, -90.0f, 0.0f)));

    // Setup decal material.
    SelectionCircleMaterialInstance = UMaterialInstanceDynamic::Create(SelectionCircleMaterial, this);
    DecalComponent->SetDecalMaterial(SelectionCircleMaterialInstance);

    DecalComponent->SetHiddenInGame(true);
}

void URTSSelectableComponent::SelectActor()
{
	if (bSelected)
	{
		return;
	}

	bSelected = true;

    // Update selection circle.
    if (IsValid(DecalComponent))
    {
        DecalComponent->SetHiddenInGame(false);
    }

	// Notify listeners.
	OnSelected.Broadcast(GetOwner());
}

void URTSSelectableComponent::DeselectActor()
{
	if (!bSelected)
	{
		return;
	}

	bSelected = false;

    // Update selection circles.
    if (IsValid(DecalComponent))
    {
        DecalComponent->SetHiddenInGame(true);
    }

	// Notify listeners.
	OnDeselected.Broadcast(GetOwner());
}

bool URTSSelectableComponent::IsSelected() const
{
	return bSelected;
}

USoundCue* URTSSelectableComponent::GetSelectedSound() const
{
    return SelectedSound;
}
