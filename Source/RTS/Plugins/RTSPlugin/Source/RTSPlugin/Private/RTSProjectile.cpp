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
}

void ARTSProjectile::FireAt(AActor* Target, float Damage, TSubclassOf<class UDamageType> DamageType, AController* EventInstigator, AActor* DamageCauser)
{
	if (!Target)
	{
		UE_LOG(RTSLog, Error, TEXT("No target set for projectile %s!"), *GetName());
		return;
	}

	this->Target = Target;
	this->Damage = Damage;
	this->DamageType = DamageType;
	this->EventInstigator = EventInstigator;
	this->DamageCauser = DamageCauser;

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
	if (!Target)
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

void ARTSProjectile::NotifyOnProjectileDetonated(AActor* Target, float Damage, TSubclassOf<class UDamageType> DamageType, AController* EventInstigator, AActor* DamageCauser)
{
	ReceiveOnProjectileDetonated(Target, Damage, DamageType, EventInstigator, DamageCauser);
}
