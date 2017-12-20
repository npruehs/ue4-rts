#include "RTSPluginPCH.h"
#include "RTSUtilities.h"

#include "EngineUtils.h"
#include "Landscape.h"
#include "Components/ShapeComponent.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "Engine/SCS_Node.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

#include "RTSConstructionSiteComponent.h"
#include "RTSOwnerComponent.h"
#include "RTSRequirementsComponent.h"


URTSUtilities::URTSUtilities(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}


float URTSUtilities::GetActorDistance(AActor* First, AActor* Second, bool bConsiderCollisionSize)
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

float URTSUtilities::GetCollisionSize(TSubclassOf<AActor> ActorClass)
{
    UShapeComponent* ShapeComponent = FindDefaultComponentByClass<UShapeComponent>(ActorClass);
    return GetShapeCollisionSize(ShapeComponent);
}

float URTSUtilities::GetCollisionHeight(TSubclassOf<AActor> ActorClass)
{
    UShapeComponent* ShapeComponent = FindDefaultComponentByClass<UShapeComponent>(ActorClass);
    return GetShapeCollisionHeight(ShapeComponent);
}

float URTSUtilities::GetActorCollisionSize(AActor* Actor)
{
    if (!Actor)
    {
        return 0.0f;
    }

    UShapeComponent* ShapeComponent = Actor->FindComponentByClass<UShapeComponent>();
    return GetShapeCollisionSize(ShapeComponent);
}

float URTSUtilities::GetActorCollisionHeight(AActor* Actor)
{
    if (!Actor)
    {
        return 0.0f;
    }

    UShapeComponent* ShapeComponent = Actor->FindComponentByClass<UShapeComponent>();
    return GetShapeCollisionHeight(ShapeComponent);
}

float URTSUtilities::GetShapeCollisionSize(UShapeComponent* ShapeComponent)
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

float URTSUtilities::GetShapeCollisionHeight(UShapeComponent* ShapeComponent)
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

FVector URTSUtilities::GetGroundLocation(UObject* WorldContextObject, FVector Location)
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

bool URTSUtilities::IsReadyToUse(AActor* Actor)
{
	if (!Actor)
	{
		return false;
	}

	auto ConstructionSiteComponent = Actor->FindComponentByClass<URTSConstructionSiteComponent>();

	return ConstructionSiteComponent == nullptr || ConstructionSiteComponent->IsFinished();
}

bool URTSUtilities::IsSuitableLocationForActor(UWorld* World, TSubclassOf<AActor> ActorClass, const FVector& Location)
{
    if (!World)
    {
        return false;
    }

    UShapeComponent* ShapeComponent = FindDefaultComponentByClass<UShapeComponent>(ActorClass);

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

UActorComponent* URTSUtilities::FindDefaultComponentByClass(const TSubclassOf<AActor> InActorClass, const TSubclassOf<UActorComponent> InComponentClass)
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

bool URTSUtilities::OwnerMeetsAllRequirementsFor(UObject* WorldContextObject, AActor* OwnedActor, TSubclassOf<AActor> DesiredProduct)
{
    TSubclassOf<AActor> MissingRequirement;
    return !GetMissingRequirementFor(WorldContextObject, OwnedActor, DesiredProduct, MissingRequirement);
}

bool URTSUtilities::GetMissingRequirementFor(UObject* WorldContextObject, AActor* OwnedActor, TSubclassOf<AActor> DesiredProduct, TSubclassOf<AActor>& OutMissingRequirement)
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

    if (!RequirementsComponent || RequirementsComponent->RequiredActors.Num() <= 0)
    {
        return false;
    }

    // Check if owning player owns all required actors.
    TArray<TSubclassOf<AActor>> RequiredActors = RequirementsComponent->RequiredActors;

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
