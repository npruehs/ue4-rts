#include "Vision/RTSVisibleComponent.h"

#include "Engine/Engine.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

#include "RTSContainableComponent.h"
#include "RTSGameState.h"
#include "RTSOwnerComponent.h"
#include "Libraries/RTSGameplayTagLibrary.h"
#include "Vision/RTSVisionManager.h"


void URTSVisibleComponent::BeginPlay()
{
    Super::BeginPlay();

    AActor* Owner = GetOwner();

    if (!Owner)
    {
        return;
    }

    // Register for events.
    URTSContainableComponent* ContainableComponent = Owner->FindComponentByClass<URTSContainableComponent>();

    if (ContainableComponent)
    {
        ContainableComponent->OnContainerChanged.AddDynamic(this, &URTSVisibleComponent::OnContainerChanged);
    }

    // Register actor with game.
    ARTSGameState* GameState = Cast<ARTSGameState>(UGameplayStatics::GetGameState(this));

    if (IsValid(GameState))
    {
        ARTSVisionManager* VisionManager = GameState->GetVisionManager();

        if (IsValid(VisionManager))
        {
            VisionManager->AddVisibleActor(Owner);
        }
    }

    // Set initial state.
    bVisible = !Owner->IsHidden();
}

ERTSVisionState URTSVisibleComponent::GetVisionStateForPlayer(AController* Controller) const
{
    return VisionStates.FindRef(Controller);
}

bool URTSVisibleComponent::IsVisibleForPlayer(AController* Controller) const
{
    ERTSVisionState VisionState = GetVisionStateForPlayer(Controller);

    // Check if we are visible anyway.
    if (VisionState == ERTSVisionState::VISION_Visible)
    {
        return true;
    }

    // Check if it's a friendly unit.
    URTSOwnerComponent* OwnerComponent = GetOwner()->FindComponentByClass<URTSOwnerComponent>();
    return IsValid(OwnerComponent) && OwnerComponent->IsSameTeamAsController(Controller);
}

void URTSVisibleComponent::SetClientHideReason(const FGameplayTag& HideReason, bool bHidden)
{
    if (bHidden)
    {
        HideReasonTags.AddTag(HideReason);
    }
    else
    {
        HideReasonTags.RemoveTag(HideReason);
    }
    
    UpdateClientHiddenFlag();
}

void URTSVisibleComponent::SetClientVisionState(ERTSVisionState InVisionState)
{
    ClientVisionState = InVisionState;

    if (IsVisibleForLocalClient())
    {
        bClientWasEverSeen = true;
    }

    UpdateClientHiddenFlag();
}

void URTSVisibleComponent::SetVisionStateForPlayer(AController* Controller, ERTSVisionState InVisionState)
{
    VisionStates.Add(Controller, InVisionState);

    if (InVisionState >= ERTSVisionState::VISION_Visible)
    {
        bWasEverSeen.Add(Controller, true);
    }
}

void URTSVisibleComponent::OnContainerChanged(AActor* Actor, AActor* NewContainer)
{
    SetClientHideReason(URTSGameplayTagLibrary::HideReason_Container(), IsValid(NewContainer));
}

void URTSVisibleComponent::UpdateClientHiddenFlag()
{
    // Check for current hide reasons.
    if (!HideReasonTags.IsEmpty())
    {
        SetVisible(false);
        return;
    }

    // Check if it's a friendly unit.
    if (IsSameTeamAsLocalClient())
    {
        SetVisible(true);
        return;
    }

    if (ClientVisionState == ERTSVisionState::VISION_Unknown)
    {
        SetVisible(false);
        return;
    }

    if (ClientVisionState == ERTSVisionState::VISION_Known)
    {
        if (bClientWasEverSeen && bDontHideAfterSeen)
        {
            SetVisible(true);
        }
        else
        {
            SetVisible(false);
        }

        return;
    }

    SetVisible(IsVisibleForLocalClient());
}

bool URTSVisibleComponent::IsSameTeamAsLocalClient() const
{
    URTSOwnerComponent* OwnerComponent = GetOwner()->FindComponentByClass<URTSOwnerComponent>();
    APlayerController* LocalPlayer = GEngine->GetFirstLocalPlayerController(GetWorld());

    return IsValid(OwnerComponent) && OwnerComponent->IsSameTeamAsController(LocalPlayer);
}

bool URTSVisibleComponent::IsVisibleForLocalClient() const
{
    return ClientVisionState >= ERTSVisionState::VISION_Visible || IsSameTeamAsLocalClient();
}

void URTSVisibleComponent::SetVisible(bool bInVisible)
{
    AActor* Owner = GetOwner();

    if (!Owner)
    {
        return;
    }

    // Set flag.
    bVisible = bInVisible;

    // Update actor.
    if (!IsNetMode(NM_DedicatedServer))
    {
        Owner->SetActorHiddenInGame(!bVisible);

        for (AActor* Child : Owner->Children)
        {
            Child->SetActorHiddenInGame(!bVisible);
        }
    }
}
