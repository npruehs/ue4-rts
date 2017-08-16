#include "RTSPluginPrivatePCH.h"
#include "RTSAttackComponent.h"

#include "RTSProjectile.h"


URTSAttackComponent::URTSAttackComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URTSAttackComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	// Update cooldown timer.
	if (RemainingCooldown > 0)
	{
		RemainingCooldown -= DeltaTime;

		if (RemainingCooldown <= 0)
		{
			UE_LOG(RTSLog, Log, TEXT("Actor %s attack is ready again."), *GetOwner()->GetName());

			// Notify listeners.
			OnCooldownReady.Broadcast();
		}
	}
}

void URTSAttackComponent::UseAttack(int32 AttackIndex, AActor* Target)
{
	AActor* Owner = GetOwner();
	APawn* OwnerPawn = Cast<APawn>(Owner);
	AController* OwnerController = OwnerPawn ? OwnerPawn->GetController() : nullptr;

	if (!IsValid(Target))
	{
		return;
	}

	// Check cooldown.
	if (RemainingCooldown > 0)
	{
		return;
	}

	// Use attack.
	UE_LOG(RTSLog, Log, TEXT("Actor %s attacks %s."), *Owner->GetName(), *Target->GetName());

	const FRTSAttackData& Attack = Attacks[0];

	ARTSProjectile* SpawnedProjectile = nullptr;

	if (Attack.ProjectileClass != nullptr)
	{
		// Fire projectile.
		// Build spawn transform.
		FVector SpawnLocation = Owner->GetActorLocation();
		FRotator SpawnRotation = Owner->GetActorRotation();
		FTransform SpawnTransform = FTransform(SpawnRotation, SpawnLocation);

		// Build spawn info.
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.Instigator = OwnerPawn;
		SpawnInfo.ObjectFlags |= RF_Transient;

		// Spawn projectile.
		SpawnedProjectile = GetWorld()->SpawnActor<ARTSProjectile>(Attack.ProjectileClass, SpawnTransform, SpawnInfo);

		if (SpawnedProjectile)
		{
			UE_LOG(RTSLog, Log, TEXT("%s fired projectile %s at target %s."), *Owner->GetName(), *SpawnedProjectile->GetName(), *Target->GetName());

			// Aim at target.
			SpawnedProjectile->FireAt(
				Target,
				Attack.Damage,
				Attack.DamageType,
				OwnerController,
				Owner);
		}
	}
	else
	{
		// Deal damage immediately.
		Target->TakeDamage(Attack.Damage, FDamageEvent(Attack.DamageType), OwnerController, Owner);
	}

	// Start cooldown timer.
	RemainingCooldown = Attack.Cooldown;

	// Notify listeners.
	OnAttackUsed.Broadcast(Attack, Target, SpawnedProjectile);
}
