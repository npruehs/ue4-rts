#include "Libraries/RTSGameplayTagLibrary.h"

#include "GameFramework/Actor.h"

#include "RTSGameplayTagsComponent.h"
#include "RTSOwnerComponent.h"
#include "RTSPlayerState.h"
#include "Libraries/RTSGameplayLibrary.h"


void URTSGameplayTagLibrary::AddGameplayTag(const AActor* Actor, const FGameplayTag& Tag)
{
    URTSGameplayTagsComponent* GameplayTagsComponent = Actor->FindComponentByClass<URTSGameplayTagsComponent>();

    if (IsValid(GameplayTagsComponent))
    {
        GameplayTagsComponent->AddGameplayTag(Tag);
    }
}

FGameplayTagContainer URTSGameplayTagLibrary::GetGameplayTags(const AActor* Actor)
{
    URTSGameplayTagsComponent* GameplayTagsComponent = Actor->FindComponentByClass<URTSGameplayTagsComponent>();
    return IsValid(GameplayTagsComponent) ? GameplayTagsComponent->GetCurrentTags() : FGameplayTagContainer::EmptyContainer;
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

FGameplayTagContainer URTSGameplayTagLibrary::GetActorRelationshipTags(const AActor* Actor, const AActor* Other)
{
    FGameplayTagContainer RelationshipTags;

    if (!IsValid(Actor) || !IsValid(Other))
    {
        RelationshipTags.AddTag(Relationship_Neutral());
    }
    else if (Actor == Other)
    {
        RelationshipTags.AddTag(Relationship_Friendly());
        RelationshipTags.AddTag(Relationship_Self());
        RelationshipTags.AddTag(Relationship_Visible());
    }
    else
    {
        const URTSOwnerComponent* ActorOwnerComponent = Actor->FindComponentByClass<URTSOwnerComponent>();
        const URTSOwnerComponent* OtherOwnerComponent = Other->FindComponentByClass<URTSOwnerComponent>();

        if (!IsValid(ActorOwnerComponent) || !IsValid(OtherOwnerComponent))
        {
            RelationshipTags.AddTag(Relationship_Neutral());
        }
        else
        {
            const ARTSPlayerState* ActorPlayerState = ActorOwnerComponent->GetPlayerOwner();
            const ARTSPlayerState* OtherPlayerState = OtherOwnerComponent->GetPlayerOwner();

            FGameplayTagContainer PlayerRelationshipTags = GetPlayerRelationshipTags(ActorPlayerState, OtherPlayerState);
            RelationshipTags.AppendTags(PlayerRelationshipTags);
        }

        if (!RelationshipTags.HasTag(Relationship_Visible()) && URTSGameplayLibrary::IsVisibleForActor(Actor, Other))
        {
            RelationshipTags.AddTag(Relationship_Visible());
        }
    }

    return RelationshipTags;
}

FGameplayTagContainer URTSGameplayTagLibrary::GetPlayerRelationshipTags(const ARTSPlayerState* ActorPlayerState, const ARTSPlayerState* OtherPlayerState)
{
    FGameplayTagContainer RelationshipTags;

    if (!IsValid(ActorPlayerState) || !IsValid(OtherPlayerState))
    {
        RelationshipTags.AddTag(Relationship_Neutral());
    }
    else if (!ActorPlayerState->IsSameTeamAs(OtherPlayerState))
    {
        RelationshipTags.AddTag(Relationship_Hostile());
    }
    else
    {
        RelationshipTags.AddTag(Relationship_Friendly());
        RelationshipTags.AddTag(Relationship_Visible());

        if (ActorPlayerState->GetPlayerIndex() == OtherPlayerState->GetPlayerIndex())
        {
            RelationshipTags.AddTag(Relationship_SamePlayer());
        }
    }

    return RelationshipTags;
}

bool URTSGameplayTagLibrary::MeetsTagRequirements(const FGameplayTagContainer& Tags, const FGameplayTagContainer& RequiredTags, const FGameplayTagContainer& BlockedTags)
{
    if (RequiredTags.Num())
    {
        if (!Tags.HasAll(RequiredTags))
        {
            return false;
        }
    }

    if (BlockedTags.Num())
    {
        if (Tags.HasAny(BlockedTags))
        {
            return false;
        }
    }

    return true;
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

const FGameplayTag& URTSGameplayTagLibrary::Relationship_Neutral()
{
    static FGameplayTag Tag = FGameplayTag::RequestGameplayTag(FName(TEXT("Relationship.Neutral")));
    return Tag;
}

const FGameplayTag& URTSGameplayTagLibrary::Relationship_Hostile()
{
    static FGameplayTag Tag = FGameplayTag::RequestGameplayTag(FName(TEXT("Relationship.Hostile")));
    return Tag;
}

const FGameplayTag& URTSGameplayTagLibrary::Relationship_SamePlayer()
{
    static FGameplayTag Tag = FGameplayTag::RequestGameplayTag(FName(TEXT("Relationship.SamePlayer")));
    return Tag;
}

const FGameplayTag& URTSGameplayTagLibrary::Relationship_Self()
{
    static FGameplayTag Tag = FGameplayTag::RequestGameplayTag(FName(TEXT("Relationship.Self")));
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
