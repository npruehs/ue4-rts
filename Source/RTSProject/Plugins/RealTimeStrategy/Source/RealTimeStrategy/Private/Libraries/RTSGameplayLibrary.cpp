#include "Libraries/RTSGameplayLibrary.h"

#include "EngineUtils.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "Engine/SCS_Node.h"

#include "RTSOwnerComponent.h"
#include "RTSPlayerState.h"
#include "RTSRequirementsComponent.h"
#include "Construction/RTSConstructionSiteComponent.h"


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

    return OwnerComponent->GetPlayerOwner() && OwnerComponent->GetPlayerOwner()->bIsABot;
}

bool URTSGameplayLibrary::IsReadyToUse(AActor* Actor)
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

bool URTSGameplayLibrary::GetMissingRequirementFor(UObject* WorldContextObject, AActor* OwnedActor, TSubclassOf<AActor> DesiredProduct, TSubclassOf<AActor>& OutMissingRequirement)
{
    if (!WorldContextObject || !OwnedActor || !OwnedActor->GetOwner())
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
