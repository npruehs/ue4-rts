#include "RTSGameplayTagsComponent.h"

#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"

#include "RTSLog.h"

URTSGameplayTagsComponent::URTSGameplayTagsComponent(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
    : Super(ObjectInitializer)
{
    SetIsReplicated(true);
}

void URTSGameplayTagsComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(URTSGameplayTagsComponent, CurrentTags);
}

void URTSGameplayTagsComponent::BeginPlay()
{
    Super::BeginPlay();

    for (const FGameplayTag& Tag : InitialTags)
    {
        CurrentTags.AddTagFast(Tag);

        UE_LOG(LogRTS, Log, TEXT("Added initial gameplay tag %s for %s."), *Tag.ToString(), *GetOwner()->GetName());
    }
}

FGameplayTagContainer URTSGameplayTagsComponent::GetCurrentTags() const
{
    return CurrentTags;
}

void URTSGameplayTagsComponent::AddGameplayTag(const FGameplayTag& NewTag)
{
    CurrentTags.AddTag(NewTag);

    UE_LOG(LogRTS, Log, TEXT("Added gameplay tag %s for %s."), *NewTag.ToString(), *GetOwner()->GetName());

    NotifyOnCurrentTagsChanged();
}

void URTSGameplayTagsComponent::AddGameplayTags(const FGameplayTagContainer& NewTags)
{
    CurrentTags.AppendTags(NewTags);

    UE_LOG(LogRTS, Log, TEXT("Added gameplay tags %s for %s."), *NewTags.ToString(), *GetOwner()->GetName());

    NotifyOnCurrentTagsChanged();
}

bool URTSGameplayTagsComponent::RemoveGameplayTag(const FGameplayTag& TagToRemove)
{
    bool bRemoved = CurrentTags.RemoveTag(TagToRemove);

    if (bRemoved)
    {
        UE_LOG(LogRTS, Log, TEXT("Removed gameplay tag %s from %s."), *TagToRemove.ToString(), *GetOwner()->GetName());

        NotifyOnCurrentTagsChanged();
    }

    return bRemoved;
}

void URTSGameplayTagsComponent::RemoveGameplayTags(const FGameplayTagContainer& TagsToRemove)
{
    CurrentTags.RemoveTags(TagsToRemove);

    UE_LOG(LogRTS, Log, TEXT("Removed gameplay tags %s from %s."), *TagsToRemove.ToString(), *GetOwner()->GetName());

    NotifyOnCurrentTagsChanged();
}

void URTSGameplayTagsComponent::NotifyOnCurrentTagsChanged()
{
    CurrentTagsChanged.Broadcast(GetOwner(), CurrentTags);
}

void URTSGameplayTagsComponent::ReceivedCurrentTags()
{
    NotifyOnCurrentTagsChanged();
}
