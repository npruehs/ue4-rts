#include "Combat/RTSProjectile.h"

#include "GameFramework/Pawn.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

#include "RTSLog.h"
#include "RTSOwnerComponent.h"


ARTSProjectile::ARTSProjectile(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
    : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	ProjectileMovement->InitialSpeed = 1000.0f;

    bFired = false;

	// Enable replication.
	// This might change in the future, as we don't really care about exact projectile positions on client-side.
	bReplicates = true;

    // Set reasonable default values.
    BallisticTrajectoryFactor = 600.0f;

    AreaOfEffect = 1000.0f;
    AreaOfEffectTargetObjectTypeFilter.Add(EObjectTypeQuery::ObjectTypeQuery2); // WorldDynamic
    AreaOfEffectTargetObjectTypeFilter.Add(EObjectTypeQuery::ObjectTypeQuery3); // Pawn
    AreaOfEffectTargetClassFilter = APawn::StaticClass();
}

void ARTSProjectile::FireAt(
	AActor* ProjectileTarget,
	float ProjectileDamage,
	TSubclassOf<class UDamageType> ProjectileDamageType,
	AController* ProjectileEventInstigator,
	AActor* ProjectileDamageCauser)
{
	if (!ProjectileTarget)
	{
		UE_LOG(LogRTS, Error, TEXT("No target set for projectile %s!"), *GetName());
		return;
	}

	MulticastFireAt(ProjectileTarget, ProjectileDamage, ProjectileDamageType, ProjectileEventInstigator, ProjectileDamageCauser);
}

void ARTSProjectile::Tick(float DeltaSeconds)
{
    if (!bFired)
    {
        return;
    }

    TimeToImpact -= DeltaSeconds;

    // Update ballistic trajectory.
    if (bBallisticTrajectory)
    {
        static const float G = 9.8067f;

        // Calculate traveled distance.
        float InitialTravelTime = InitialDistance / ProjectileMovement->InitialSpeed;
        float PassedTravelTime = InitialTravelTime - TimeToImpact;
        float TraveledDistance = PassedTravelTime * ProjectileMovement->InitialSpeed;

        // Calculate current height.
        float ProjectileHeight = TraveledDistance * FMath::Tan(LaunchAngle) -
            ((G * (TraveledDistance * TraveledDistance)) /
            (2 * FMath::Pow(ProjectileMovement->InitialSpeed * FMath::Cos(LaunchAngle), 2)));

        FVector ProjectileLocation = GetActorLocation();
        ProjectileLocation.Z = InitialHeight + (ProjectileHeight * BallisticTrajectoryFactor) +
            ((TargetHeight - InitialHeight) * (PassedTravelTime / InitialTravelTime));
        SetActorLocation(ProjectileLocation);
    }

	if (TimeToImpact > 0.0f)
	{
        return;
	}

    if (HasAuthority())
    {
        if (!bApplyAreaOfEffect)
        {
            HitTargetActor(Target);
        }
        else
        {
            HitTargetLocation();
        }
    }

    // Notify listeners.
    NotifyOnProjectileDetonated(Target, Damage, DamageType, EventInstigator, DamageCauser);

    // Destroy projectile.
    Destroy();
}

void ARTSProjectile::NotifyOnProjectileDetonated(
	AActor* ProjectileTarget,
	float ProjectileDamage,
	TSubclassOf<class UDamageType> ProjectileDamageType,
	AController* ProjectileEventInstigator,
	AActor* ProjectileDamageCauser)
{
	ReceiveOnProjectileDetonated(ProjectileTarget, ProjectileDamage, ProjectileDamageType, ProjectileEventInstigator, ProjectileDamageCauser);
}

void ARTSProjectile::HitTargetActor(AActor* Actor)
{
    if (!IsValid(Actor))
    {
        return;
    }

    UE_LOG(LogRTS, Log, TEXT("Projectile %s hit target %s for %f damage."), *GetName(), *Actor->GetName(), Damage);

    // Deal damage.
    Actor->TakeDamage(Damage, FDamageEvent(DamageType), EventInstigator, DamageCauser);
}

void ARTSProjectile::HitTargetLocation()
{
    // Overlap actors in target area.
    TArray<AActor*> OverlapedActors;
    TArray<AActor*> ActorsToIgnore;

    UKismetSystemLibrary::CapsuleOverlapActors(this, FVector(TargetLocation.X, TargetLocation.Y, 0.0f),
        AreaOfEffect, 10000.0f,
        AreaOfEffectTargetObjectTypeFilter, AreaOfEffectTargetClassFilter,
        ActorsToIgnore, OverlapedActors);

    // Collect valid targets (e.g. by owner).
    for (AActor* OverlapedActor : OverlapedActors)
    {
        if (!IsValid(OverlapedActor))
        {
            continue;
        }

        // Note that we always apply the effects to the real projectile target.
        // This is necessary for forced attacks to friendly units.
        if (OverlapedActor == Target)
        {
            HitTargetActor(OverlapedActor);
            continue;
        }

        // Check owner.
        URTSOwnerComponent* OwnerComponent = OverlapedActor->FindComponentByClass<URTSOwnerComponent>();

        if (IsValid(OwnerComponent) && !OwnerComponent->IsSameTeamAsActor(DamageCauser))
        {
            HitTargetActor(OverlapedActor);
            continue;
        }
    }
}

void ARTSProjectile::MulticastFireAt_Implementation(AActor* ProjectileTarget, float ProjectileDamage,
    TSubclassOf<class UDamageType> ProjectileDamageType, AController* ProjectileEventInstigator,
    AActor* ProjectileDamageCauser)
{
    Target = ProjectileTarget;
    Damage = ProjectileDamage;
    DamageType = ProjectileDamageType;
    EventInstigator = ProjectileEventInstigator;
    DamageCauser = ProjectileDamageCauser;

    TargetLocation = Target->GetActorLocation();

    // Set direction.
    FVector Direction = Target->GetActorLocation() - GetActorLocation();
    FVector DirectionNormalized = Direction.GetSafeNormal(0.01f);

    InitialDistance = Direction.Size();
    InitialHeight = GetActorLocation().Z;
    TargetHeight = Target->GetActorLocation().Z;

    ProjectileMovement->Velocity = DirectionNormalized * ProjectileMovement->InitialSpeed;

    if (ProjectileMovement->bIsHomingProjectile)
    {
        // Set target.
        ProjectileMovement->HomingTargetComponent = Target->GetRootComponent();
    }

    // Set time to impact.
    TimeToImpact = InitialDistance / ProjectileMovement->InitialSpeed;
    bFired = true;

    // Setup ballistic trajectory.
    if (bBallisticTrajectory)
    {
        // Calculate angle of reach.
        static const float G = 9.8067f;
        LaunchAngle = 0.5f * FMath::Asin(G * InitialDistance / (ProjectileMovement->InitialSpeed * ProjectileMovement->InitialSpeed));
    }

    // Clients will take it from here.
    TearOff();
}
