#include "Combat/RTSProjectileTargetComponent.h"

#include "Components/MeshComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"

#include "RTSLog.h"


void URTSProjectileTargetComponent::BeginPlay()
{
    Super::BeginPlay();

    AActor* Owner = GetOwner();

    if (!IsValid(Owner))
    {
        return;
    }

    // Find tagged primary mesh.
    if (PrimaryMeshComponentTag.IsValid())
    {
        for (UActorComponent* ActorComponent : Owner->GetComponentsByTag(UMeshComponent::StaticClass(), PrimaryMeshComponentTag))
        {
            MeshComponent = Cast<UMeshComponent>(ActorComponent);
            break;
        }
    }

    // Fall back to any mesh.
    if (!IsValid(MeshComponent))
    {
        MeshComponent = Owner->FindComponentByClass<UMeshComponent>();
    }

    if (!IsValid(MeshComponent))
    {
        UE_LOG(LogRTS, Warning, TEXT("%s has a ProjectileTargetComponent attached, but no MeshComponent."),
            *Owner->GetName());
    }
}

FVector URTSProjectileTargetComponent::GetRandomProjectileTargetLocation() const
{
    AActor* Owner = GetOwner();

    if (!IsValid(Owner))
    {
        return FVector::ZeroVector;
    }

    if (!IsValid(MeshComponent))
    {
        return Owner->GetActorLocation();
    }

    if (TargetSockets.Num() <= 0)
    {
        return Owner->GetActorLocation();
    }

    int32 RandomIndex = UKismetMathLibrary::RandomIntegerInRange(0, TargetSockets.Num() - 1);
    FName TargetSocketName = TargetSockets[RandomIndex];
    return MeshComponent->GetSocketLocation(TargetSocketName);
}
