#include "Libraries/RTSGameplayTagLibrary.h"

#include "GameFramework/Actor.h"

#include "RTSGameplayTagsComponent.h"


void URTSGameplayTagLibrary::AddGameplayTag(const AActor* Actor, const FGameplayTag& Tag)
{
    URTSGameplayTagsComponent* GameplayTagsComponent = Actor->FindComponentByClass<URTSGameplayTagsComponent>();

    if (IsValid(GameplayTagsComponent))
    {
        GameplayTagsComponent->AddGameplayTag(Tag);
    }
}

bool URTSGameplayTagLibrary::HasGameplayTag(const AActor* Actor, const FGameplayTag& Tag)
{
    if (!IsValid(Actor))
    {
        return false;
    }

    URTSGameplayTagsComponent* GameplayTagsComponent = Actor->FindComponentByClass<URTSGameplayTagsComponent>();
    return IsValid(GameplayTagsComponent) ? GameplayTagsComponent->GetCurrentTags().HasTag(Tag) : false;
}

void URTSGameplayTagLibrary::RemoveGameplayTag(const AActor* Actor, const FGameplayTag& Tag)
{
    URTSGameplayTagsComponent* GameplayTagsComponent = Actor->FindComponentByClass<URTSGameplayTagsComponent>();

    if (IsValid(GameplayTagsComponent))
    {
        GameplayTagsComponent->RemoveGameplayTag(Tag);
    }
}

const FGameplayTag& URTSGameplayTagLibrary::Container_ConstructionSite()
{
    static FGameplayTag Tag = FGameplayTag::RequestGameplayTag(FName(TEXT("Container.ConstructionSite")));
    return Tag;
}

const FGameplayTag& URTSGameplayTagLibrary::Container_ResourceSource()
{
    static FGameplayTag Tag = FGameplayTag::RequestGameplayTag(FName(TEXT("Container.ResourceSource")));
    return Tag;
}

const FGameplayTag& URTSGameplayTagLibrary::HideReason_Container()
{
    static FGameplayTag Tag = FGameplayTag::RequestGameplayTag(FName(TEXT("HideReason.Container")));
    return Tag;
}

const FGameplayTag& URTSGameplayTagLibrary::Relationship_Friendly()
{
    static FGameplayTag Tag = FGameplayTag::RequestGameplayTag(FName(TEXT("Relationship.Friendly")));
    return Tag;
}

const FGameplayTag& URTSGameplayTagLibrary::Relationship_SamePlayer()
{
    static FGameplayTag Tag = FGameplayTag::RequestGameplayTag(FName(TEXT("Relationship.SamePlayer")));
    return Tag;
}

const FGameplayTag& URTSGameplayTagLibrary::Relationship_Visible()
{
    static FGameplayTag Tag = FGameplayTag::RequestGameplayTag(FName(TEXT("Relationship.Visible")));
    return Tag;
}

const FGameplayTag& URTSGameplayTagLibrary::Status_Changing_Alive()
{
    static FGameplayTag Tag = FGameplayTag::RequestGameplayTag(FName(TEXT("Status.Changing.Alive")));
    return Tag;
}

const FGameplayTag& URTSGameplayTagLibrary::Status_Changing_Constructing()
{
    static FGameplayTag Tag = FGameplayTag::RequestGameplayTag(FName(TEXT("Status.Changing.Constructing")));
    return Tag;
}

const FGameplayTag& URTSGameplayTagLibrary::Status_Changing_Immobilized()
{
    static FGameplayTag Tag = FGameplayTag::RequestGameplayTag(FName(TEXT("Status.Changing.Immobilized")));
    return Tag;
}

const FGameplayTag& URTSGameplayTagLibrary::Status_Changing_Invulnerable()
{
    static FGameplayTag Tag = FGameplayTag::RequestGameplayTag(FName(TEXT("Status.Changing.Invulnerable")));
    return Tag;
}

const FGameplayTag& URTSGameplayTagLibrary::Status_Changing_Unarmed()
{
    static FGameplayTag Tag = FGameplayTag::RequestGameplayTag(FName(TEXT("Status.Changing.Unarmed")));
    return Tag;
}

const FGameplayTag& URTSGameplayTagLibrary::Status_Changing_UnderConstruction()
{
    static FGameplayTag Tag = FGameplayTag::RequestGameplayTag(FName(TEXT("Status.Changing.UnderConstruction")));
    return Tag;
}

const FGameplayTag& URTSGameplayTagLibrary::Status_Permanent_CanAttack()
{
    static FGameplayTag Tag = FGameplayTag::RequestGameplayTag(FName(TEXT("Status.Permanent.CanAttack")));
    return Tag;
}

const FGameplayTag& URTSGameplayTagLibrary::Status_Permanent_CanBeAttacked()
{
    static FGameplayTag Tag = FGameplayTag::RequestGameplayTag(FName(TEXT("Status.Permanent.CanBeAttacked")));
    return Tag;
}

const FGameplayTag& URTSGameplayTagLibrary::Status_Permanent_CanConstruct()
{
    static FGameplayTag Tag = FGameplayTag::RequestGameplayTag(FName(TEXT("Status.Permanent.CanConstruct")));
    return Tag;
}

const FGameplayTag& URTSGameplayTagLibrary::Status_Permanent_CanBeConstructed()
{
    static FGameplayTag Tag = FGameplayTag::RequestGameplayTag(FName(TEXT("Status.Permanent.CanBeConstructed")));
    return Tag;
}

const FGameplayTag& URTSGameplayTagLibrary::Status_Permanent_CanGather()
{
    static FGameplayTag Tag = FGameplayTag::RequestGameplayTag(FName(TEXT("Status.Permanent.CanGather")));
    return Tag;
}

const FGameplayTag& URTSGameplayTagLibrary::Status_Permanent_CanBeGathered()
{
    static FGameplayTag Tag = FGameplayTag::RequestGameplayTag(FName(TEXT("Status.Permanent.CanBeGathered")));
    return Tag;
}

const FGameplayTag& URTSGameplayTagLibrary::Status_Permanent_CanProduce()
{
    static FGameplayTag Tag = FGameplayTag::RequestGameplayTag(FName(TEXT("Status.Permanent.CanProduce")));
    return Tag;
}
