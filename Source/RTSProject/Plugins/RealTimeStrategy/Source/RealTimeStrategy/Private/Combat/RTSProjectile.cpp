#include "Combat/RTSProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"

#include "RTSLog.h"


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

    if (IsValid(Target))
    {
        if (HasAuthority())
        {
            UE_LOG(LogRTS, Log, TEXT("Projectile %s hit target %s for %f damage."), *GetName(), *Target->GetName(), Damage);

            // Deal damage.
            Target->TakeDamage(Damage, FDamageEvent(DamageType), EventInstigator, DamageCauser);

            // Notify listeners.
            NotifyOnProjectileDetonated(Target, Damage, DamageType, EventInstigator, DamageCauser);
        }
    }

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

void ARTSProjectile::MulticastFireAt_Implementation(AActor* ProjectileTarget, float ProjectileDamage,
    TSubclassOf<class UDamageType> ProjectileDamageType, AController* ProjectileEventInstigator,
    AActor* ProjectileDamageCauser)
{
    Target = ProjectileTarget;
    Damage = ProjectileDamage;
    DamageType = ProjectileDamageType;
    EventInstigator = ProjectileEventInstigator;
    DamageCauser = ProjectileDamageCauser;

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
