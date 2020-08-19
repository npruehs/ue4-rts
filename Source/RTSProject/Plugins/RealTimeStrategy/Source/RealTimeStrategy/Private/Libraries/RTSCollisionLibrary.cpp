#include "Libraries/RTSCollisionLibrary.h"

#include "Landscape.h"
#include "Components/ShapeComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/SCS_Node.h"

#include "RTSLog.h"
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
    if (ActorClass == nullptr)
    {
        return 0.0;
    }

    AActor* DefaultActor = ActorClass->GetDefaultObject<AActor>();
    return GetActorCollisionSize(DefaultActor) * DefaultActor->GetActorRelativeScale3D().X;
}

float URTSCollisionLibrary::GetCollisionHeight(TSubclassOf<AActor> ActorClass)
{
    if (ActorClass == nullptr)
    {
        return 0.0;
    }

    AActor* DefaultActor = ActorClass->GetDefaultObject<AActor>();
    return GetActorCollisionHeight(DefaultActor) * DefaultActor->GetActorRelativeScale3D().Z;
}

UActorComponent* URTSCollisionLibrary::FindDefaultComponentByClass(const TSubclassOf<AActor> InActorClass,
	const TSubclassOf<UActorComponent> InComponentClass)
{
	if (!IsValid(InActorClass))
	{
		return nullptr;
	}

	// Check CDO.
	AActor* ActorCDO = InActorClass->GetDefaultObject<AActor>();
	UActorComponent* FoundComponent = ActorCDO->FindComponentByClass(InComponentClass);

	if (FoundComponent != nullptr)
	{
		return FoundComponent;
	}

	// Check blueprint nodes. Components added in blueprint editor only (and not in code) are not available from
	// CDO.
	UBlueprintGeneratedClass* RootBlueprintGeneratedClass = Cast<UBlueprintGeneratedClass>(InActorClass);
	UClass* ActorClass = InActorClass;

	// Go down the inheritance tree to find nodes that were added to parent blueprints of our blueprint graph.
	do
	{
		UBlueprintGeneratedClass* ActorBlueprintGeneratedClass = Cast<UBlueprintGeneratedClass>(ActorClass);
		if (!ActorBlueprintGeneratedClass)
		{
			return nullptr;
		}

		const TArray<USCS_Node*>& ActorBlueprintNodes =
			ActorBlueprintGeneratedClass->SimpleConstructionScript->GetAllNodes();

		for (USCS_Node* Node : ActorBlueprintNodes)
		{
			if (Node->ComponentClass->IsChildOf(InComponentClass))
			{
				return Node->GetActualComponentTemplate(RootBlueprintGeneratedClass);
			}
		}

		ActorClass = Cast<UClass>(ActorClass->GetSuperStruct());

	} while (ActorClass != AActor::StaticClass());

	return nullptr;
}

float URTSCollisionLibrary::GetActorCollisionSize(AActor* Actor)
{
    if (!Actor)
    {
        return 0.0f;
    }

	const TSubclassOf<AActor> ActorClass = Actor->GetClass();
	UShapeComponent* ShapeComponent = FindDefaultComponentByClass<UShapeComponent>(ActorClass);
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

    if (CollisionShape.IsBox())
    {
        return FMath::Max(CollisionShape.Box.HalfExtentX, CollisionShape.Box.HalfExtentY) * 2;
    }
    else if (CollisionShape.IsCapsule())
    {
        return CollisionShape.Capsule.Radius * 2;
    }
    else if (CollisionShape.IsSphere())
    {
        return CollisionShape.Sphere.Radius * 2;
    }
    else
    {
        UE_LOG(LogRTS, Error, TEXT("Unknown collision shape type for %s."), *ShapeComponent->GetOwner()->GetName());
        return 0.0f;
    }
}

float URTSCollisionLibrary::GetShapeCollisionHeight(UShapeComponent* ShapeComponent)
{
    if (!ShapeComponent)
    {
        return 0.0f;
    }

    FCollisionShape CollisionShape = ShapeComponent->GetCollisionShape();

    if (CollisionShape.IsBox())
    {
        return CollisionShape.Box.HalfExtentZ * 2;
    }
    else if (CollisionShape.IsCapsule())
    {
        return CollisionShape.Capsule.HalfHeight * 2;
    }
    else if (CollisionShape.IsSphere())
    {
        return CollisionShape.Sphere.Radius * 2;
    }
    else
    {
        UE_LOG(LogRTS, Error, TEXT("Unknown collision shape type for %s."), *ShapeComponent->GetOwner()->GetName());
        return 0.0f;
    }
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
