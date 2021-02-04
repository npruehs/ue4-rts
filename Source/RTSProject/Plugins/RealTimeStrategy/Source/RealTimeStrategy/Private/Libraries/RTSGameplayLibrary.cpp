#include "Libraries/RTSGameplayLibrary.h"

#include "EngineUtils.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "Engine/SCS_Node.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Runtime/Launch/Resources/Version.h"

#include "RTSOwnerComponent.h"
#include "RTSPlayerState.h"
#include "RTSRequirementsComponent.h"
#include "RTSSelectableComponent.h"
#include "Combat/RTSAttackComponent.h"
#include "Construction/RTSConstructionSiteComponent.h"
#include "Economy/RTSGathererComponent.h"
#include "Vision/RTSVisibleComponent.h"


UActorComponent* URTSGameplayLibrary::FindDefaultComponentByClass(const TSubclassOf<AActor> InActorClass, const TSubclassOf<UActorComponent> InComponentClass)
{
    // Check CDO.
    AActor* ActorCDO = InActorClass->GetDefaultObject<AActor>();;
    UActorComponent* FoundComponent = ActorCDO->FindComponentByClass(InComponentClass);

    if (FoundComponent)
    {
        return FoundComponent;
    }

    // Check blueprint nodes. Components added in blueprint editor only (and not in code) are not available from CDO.
    UBlueprintGeneratedClass* ActorBlueprintGeneratedClass = Cast<UBlueprintGeneratedClass>(InActorClass);

    if (!ActorBlueprintGeneratedClass)
    {
        return nullptr;
    }

    const TArray<USCS_Node*>& ActorBlueprintNodes = ActorBlueprintGeneratedClass->SimpleConstructionScript->GetAllNodes();

    for (USCS_Node* Node : ActorBlueprintNodes)
    {
        if (Node->ComponentClass->IsChildOf(InComponentClass))
        {
            return Node->ComponentTemplate;
        }
    }

    return nullptr;
}

bool URTSGameplayLibrary::IsAIUnit(AActor* Actor)
{
    if (!Actor)
    {
        return false;
    }

    // Check owner.
    auto OwnerComponent = Actor->FindComponentByClass<URTSOwnerComponent>();

    if (!OwnerComponent)
    {
        return false;
    }

    return IsOwnerABot(OwnerComponent);

}

bool URTSGameplayLibrary::IsReadyToUse(const AActor* Actor)
{
    if (!Actor)
    {
        return false;
    }

    auto ConstructionSiteComponent = Actor->FindComponentByClass<URTSConstructionSiteComponent>();

    return ConstructionSiteComponent == nullptr || ConstructionSiteComponent->IsFinished();
}

bool URTSGameplayLibrary::OwnerMeetsAllRequirementsFor(UObject* WorldContextObject, AActor* OwnedActor, TSubclassOf<AActor> DesiredProduct)
{
    TSubclassOf<AActor> MissingRequirement;
    return !GetMissingRequirementFor(WorldContextObject, OwnedActor, DesiredProduct, MissingRequirement);
}

void URTSGameplayLibrary::StopGameplayFor(AActor* Actor)
{
    if (!IsValid(Actor))
    {
        return;
    }

    Actor->SetActorEnableCollision(false);

    UCharacterMovementComponent* CharacterMovement = Actor->FindComponentByClass<UCharacterMovementComponent>();

    if (CharacterMovement != nullptr)
    {
        CharacterMovement->SetMovementMode(MOVE_None);
    }

    DestroyComponentByClass<URTSSelectableComponent>(Actor);
    DestroyComponentByClass<URTSAttackComponent>(Actor);
    DestroyComponentByClass<URTSGathererComponent>(Actor);
    DestroyComponentByClass<URTSConstructionSiteComponent>(Actor);
}

bool URTSGameplayLibrary::IsVisibleForActor(const AActor* Actor, const AActor* Other)
{
    if (!IsValid(Actor) || !IsValid(Other))
    {
        return false;
    }

    const URTSVisibleComponent* OtherVisibleComponent = Other->FindComponentByClass<URTSVisibleComponent>();

    if (OtherVisibleComponent != nullptr)
    {
        if (Other->HasAuthority())
        {
            return OtherVisibleComponent->IsVisibleForPlayer(Cast<AController>(Actor->GetOwner()));
        }

        return OtherVisibleComponent->IsVisibleForLocalClient();
    }
    else
    {
        // No visible component? It must be always visible!
        return true;
    }
}

bool URTSGameplayLibrary::IsFullyVisibleForLocalClient(const AActor* Actor)
{
    if (!IsValid(Actor))
    {
        return false;
    }

    // Check if visible.
    if (Actor->IsHidden())
    {
        return false;
    }

    URTSVisibleComponent* VisibleComponent = Actor->FindComponentByClass<URTSVisibleComponent>();

    if (IsValid(VisibleComponent) && !VisibleComponent->IsVisibleForLocalClient())
    {
        return false;
    }

    return true;
}

bool URTSGameplayLibrary::GetMissingRequirementFor(UObject* WorldContextObject, AActor* OwnedActor, TSubclassOf<AActor> DesiredProduct, TSubclassOf<AActor>& OutMissingRequirement)
{
    if (!WorldContextObject || !OwnedActor || !OwnedActor->GetOwner())
    {
        return false;
    }

    if (DesiredProduct == nullptr)
    {
        return false;
    }

    // Check owner.
    URTSOwnerComponent* OwnerComponent = OwnedActor->FindComponentByClass<URTSOwnerComponent>();

    if (OwnerComponent == nullptr)
    {
        return false;
    }

    // Check if any requirements.
    URTSRequirementsComponent* RequirementsComponent = FindDefaultComponentByClass<URTSRequirementsComponent>(DesiredProduct);

    if (!RequirementsComponent || RequirementsComponent->GetRequiredActors().Num() <= 0)
    {
        return false;
    }

    // Check if owning player owns all required actors.
    TArray<TSubclassOf<AActor>> RequiredActors = RequirementsComponent->GetRequiredActors();

    for (TActorIterator<AActor> ActorItr(WorldContextObject->GetWorld()); ActorItr; ++ActorItr)
    {
        AActor* SomeActor = *ActorItr;
        URTSOwnerComponent* OtherOwnerComponent = SomeActor->FindComponentByClass<URTSOwnerComponent>();

        if (OtherOwnerComponent && OtherOwnerComponent->GetPlayerOwner() == OwnerComponent->GetPlayerOwner() && IsReadyToUse(SomeActor))
        {
            RequiredActors.Remove(SomeActor->GetClass());

            if (RequiredActors.Num() == 0)
            {
                // All requirements checked.
                return false;
            }
        }
    }

    OutMissingRequirement = RequiredActors[0];
    return true;
}

bool URTSGameplayLibrary::IsOwnedByLocalPlayer(AActor* Actor)
{
    if (!IsValid(Actor))
    {
        return false;
    }

    URTSOwnerComponent* OwnerComponent = Actor->FindComponentByClass<URTSOwnerComponent>();

    if (!IsValid(OwnerComponent))
    {
        return false;
    }

    UWorld* World = Actor->GetWorld();
    
    if (!IsValid(World))
    {
        return false;
    }
    
    APlayerController* PlayerController = World->GetFirstPlayerController();

    if (!IsValid(PlayerController))
    {
        return false;
    }

    return OwnerComponent->GetPlayerOwner() == PlayerController->GetPlayerState<ARTSPlayerState>();
}

bool URTSGameplayLibrary::IsOwnerABot(URTSOwnerComponent* OwnerComponent)
{
	#if ENGINE_MINOR_VERSION < 25
		return OwnerComponent->GetPlayerOwner() && OwnerComponent->GetPlayerOwner()->bIsABot;
	#else
		return OwnerComponent->GetPlayerOwner() && OwnerComponent->GetPlayerOwner()->IsABot();
	#endif
}
