#include "RTSContainableComponent.h"

#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"

#include "RTSContainerComponent.h"
#include "RTSLog.h"
#include "Combat/RTSHealthComponent.h"


URTSContainableComponent::URTSContainableComponent(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
    : Super(ObjectInitializer)
{
    SetIsReplicatedByDefault(true);
}

void URTSContainableComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(URTSContainableComponent, Container);
}

void URTSContainableComponent::BeginPlay()
{
    Super::BeginPlay();

    AActor* Owner = GetOwner();

    if (!IsValid(Owner))
    {
        return;
    }

    URTSHealthComponent* HealthComponent = Owner->FindComponentByClass<URTSHealthComponent>();

    if (!IsValid(HealthComponent))
    {
        return;
    }

    HealthComponent->OnKilled.AddDynamic(this, &URTSContainableComponent::OnKilled);
}

AActor* URTSContainableComponent::GetContainer() const
{
    return Container;
}

void URTSContainableComponent::SetContainer(AActor* NewContainer)
{
    if (Container == NewContainer)
    {
        return;
    }

    Container = NewContainer;

    // Notify listeners.
    OnContainerChanged.Broadcast(GetOwner(), NewContainer);
}

void URTSContainableComponent::ReceivedContainer()
{
    // Notify listeners.
    OnContainerChanged.Broadcast(GetOwner(), Container);
}

void URTSContainableComponent::OnKilled(AActor* Actor, AController* PreviousOwner, AActor* DamageCauser)
{
    if (!IsValid(Container))
    {
        return;
    }

    AActor* Owner = GetOwner();

    if (!IsValid(Owner))
    {
        return;
    }

    // Unload from all containers.
    TArray<URTSContainerComponent*> ContainerComponents;
    Container->GetComponents(ContainerComponents);

    for (URTSContainerComponent* ContainerComponent : ContainerComponents)
    {
        if (ContainerComponent->ContainsActor(Owner))
        {
            UE_LOG(LogRTS, Log, TEXT("Unloading actor %s from container %s because it was killed while being inside."),
                *Owner->GetName(), *Container->GetName());

            ContainerComponent->UnloadActor(Owner);
        }
    }
}
