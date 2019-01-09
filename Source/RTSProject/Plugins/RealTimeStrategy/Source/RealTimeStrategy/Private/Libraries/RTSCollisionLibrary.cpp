#include "RealTimeStrategyPCH.h"
#include "Libraries/RTSCollisionLibrary.h"

#include "Landscape.h"
#include "Components/ShapeComponent.h"
#include "GameFramework/Actor.h"

#include "Libraries/RTSGameplayLibrary.h"


float URTSCollisionLibrary::GetActorDistance(AActor* First, AActor* Second, bool bConsiderCollisionSize)
{
    if (!First || !Second)
    {
        return 0.0f;
    }

    float Distance = First->GetDistanceTo(Second);

    if (bConsiderCollisionSize)
    {
        Distance -= GetActorCollisionSize(First) / 2.0f;
        Distance -= GetActorCollisionSize(Second) / 2.0f;
    }

    return Distance;
}

float URTSCollisionLibrary::GetCollisionSize(TSubclassOf<AActor> ActorClass)
{
    AActor* DefaultActor = ActorClass->GetDefaultObject<AActor>();
    return GetActorCollisionSize(DefaultActor) * DefaultActor->GetActorRelativeScale3D().X;
}

float URTSCollisionLibrary::GetCollisionHeight(TSubclassOf<AActor> ActorClass)
{
    AActor* DefaultActor = ActorClass->GetDefaultObject<AActor>();
    return GetActorCollisionHeight(DefaultActor) * DefaultActor->GetActorRelativeScale3D().Z;
}

float URTSCollisionLibrary::GetActorCollisionSize(AActor* Actor)
{
    if (!Actor)
    {
        return 0.0f;
    }

    UShapeComponent* ShapeComponent = Actor->FindComponentByClass<UShapeComponent>();
    return GetShapeCollisionSize(ShapeComponent);
}

float URTSCollisionLibrary::GetActorCollisionHeight(AActor* Actor)
{
    if (!Actor)
    {
        return 0.0f;
    }

    UShapeComponent* ShapeComponent = Actor->FindComponentByClass<UShapeComponent>();
    return GetShapeCollisionHeight(ShapeComponent);
}

float URTSCollisionLibrary::GetShapeCollisionSize(UShapeComponent* ShapeComponent)
{
    if (!ShapeComponent)
    {
        return 0.0f;
    }

    FCollisionShape CollisionShape = ShapeComponent->GetCollisionShape();

    return CollisionShape.IsCapsule() ?
        CollisionShape.Capsule.Radius * 2 :
        FMath::Max(CollisionShape.Box.HalfExtentX, CollisionShape.Box.HalfExtentY) * 2;
}

float URTSCollisionLibrary::GetShapeCollisionHeight(UShapeComponent* ShapeComponent)
{
    if (!ShapeComponent)
    {
        return 0.0f;
    }

    FCollisionShape CollisionShape = ShapeComponent->GetCollisionShape();

    return CollisionShape.IsCapsule() ?
        CollisionShape.Capsule.HalfHeight * 2 :
        CollisionShape.Box.HalfExtentZ * 2;
}

FVector URTSCollisionLibrary::GetGroundLocation(UObject* WorldContextObject, FVector Location)
{
    if (!WorldContextObject)
    {
        return Location;
    }

    // Cast ray to hit world.
    FCollisionObjectQueryParams Params(FCollisionObjectQueryParams::InitType::AllObjects);
    TArray<FHitResult> HitResults;

    WorldContextObject->GetWorld()->LineTraceMultiByObjectType(
        HitResults,
        FVector(Location.X, Location.Y, 10000.0f),
        FVector(Location.X, Location.Y, -10000.0f),
        Params);

    for (auto& HitResult : HitResults)
    {
        if (HitResult.Actor != nullptr)
        {
            ALandscape* Landscape = Cast<ALandscape>(HitResult.Actor.Get());

            if (Landscape != nullptr)
            {
                return HitResult.Location;
            }

            continue;
        }

        return HitResult.Location;
    }

    return Location;
}

bool URTSCollisionLibrary::IsSuitableLocationForActor(UWorld* World, TSubclassOf<AActor> ActorClass, const FVector& Location)
{
    if (!World)
    {
        return false;
    }

    UShapeComponent* ShapeComponent = URTSGameplayLibrary::FindDefaultComponentByClass<UShapeComponent>(ActorClass);

    if (!ShapeComponent)
    {
        return true;
    }

    FCollisionObjectQueryParams Params(FCollisionObjectQueryParams::AllDynamicObjects);

    return !World->OverlapAnyTestByObjectType(
        Location,
        FQuat::Identity,
        Params,
        ShapeComponent->GetCollisionShape());
}
