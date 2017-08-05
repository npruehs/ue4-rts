#include "RTSPluginPrivatePCH.h"
#include "RTSProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"


ARTSProjectile::ARTSProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale = 0.0f;

	ImpactThresholdSquared = 100.0f;

	// Enable replication.
	// This might change in the future, as we don't really care about exact projectile positions on client-side.
	bReplicates = true;
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
		UE_LOG(RTSLog, Error, TEXT("No target set for projectile %s!"), *GetName());
		return;
	}

	Target = ProjectileTarget;
	Damage = ProjectileDamage;
	DamageType = ProjectileDamageType;
	EventInstigator = ProjectileEventInstigator;
	DamageCauser = ProjectileDamageCauser;

	// Set direction.
	FVector Direction = Target->GetActorLocation() - GetActorLocation();
	FVector DirectionNormalized = Direction.GetSafeNormal(0.01f);

	ProjectileMovement->Velocity = DirectionNormalized * ProjectileMovement->InitialSpeed;

	if (ProjectileMovement->bIsHomingProjectile)
	{
		// Set target.
		ProjectileMovement->HomingTargetComponent = Target->GetRootComponent();
	}
}

void ARTSProjectile::Tick(float DeltaSeconds)
{
	if (!IsValid(Target))
	{
		return;
	}

	if (FVector::DistSquared(GetActorLocation(), Target->GetActorLocation()) < ImpactThresholdSquared)
	{
		UE_LOG(RTSLog, Log, TEXT("Projectile %s hit target %s for %f damage."), *GetName(), *Target->GetName(), Damage);

		// Deal damage.
		Target->TakeDamage(Damage, FDamageEvent(DamageType), EventInstigator, DamageCauser);

		// Notify listeners.
		NotifyOnProjectileDetonated(Target, Damage, DamageType, EventInstigator, DamageCauser);

		// Destroy projectile.
		Destroy();
	}
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
