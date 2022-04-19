#include "Combat/RTSAttackComponent.h"

#include "Engine/World.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"

#include "RTSLog.h"
#include "RTSPlayerAdvantageComponent.h"
#include "Combat/RTSProjectile.h"
#include "Libraries/RTSGameplayTagLibrary.h"


URTSAttackComponent::URTSAttackComponent(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;

	// Set reasonable default values.
	AcquisitionRadius = 1000.0f;
	ChaseRadius = 1000.0f;

	FRTSAttackData DefaultAttack;
	DefaultAttack.Cooldown = 0.5f;
	DefaultAttack.Damage = 10.0f;
	DefaultAttack.Range = 200.0f;

	Attacks.Add(DefaultAttack);

	InitialGameplayTags.AddTag(URTSGameplayTagLibrary::Status_Permanent_CanAttack());
}

void URTSAttackComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	// Update cooldown timer.
	if (RemainingCooldown > 0)
	{
		RemainingCooldown -= DeltaTime;

		if (RemainingCooldown <= 0)
		{
			UE_LOG(LogRTS, Log, TEXT("Actor %s attack is ready again."), *GetOwner()->GetName());

			// Notify listeners.
			OnCooldownReady.Broadcast(GetOwner());
		}
	}
}

void URTSAttackComponent::UseAttack(int32 AttackIndex, AActor* Target)
{
	AActor* Owner = GetOwner();
	APawn* OwnerPawn = Cast<APawn>(Owner);
	AController* OwnerController = Cast<AController>(Owner->GetOwner());

	if (!IsValid(Target))
	{
		return;
	}

	// Check cooldown.
	if (RemainingCooldown > 0)
	{
		return;
	}

	// Calculate damage.
	if (!Attacks.IsValidIndex(AttackIndex))
	{
		return;
	}

	const FRTSAttackData& Attack = Attacks[AttackIndex];

	float Damage = Attack.Damage;

	if (IsValid(OwnerController))
	{
		const URTSPlayerAdvantageComponent* PlayerAdvantageComponent = OwnerController->FindComponentByClass<URTSPlayerAdvantageComponent>();

		if (IsValid(PlayerAdvantageComponent))
		{
			Damage *= PlayerAdvantageComponent->GetOutgoingDamageFactor();
		}
	}

	// Use attack.
	UE_LOG(LogRTS, Log, TEXT("Actor %s attacks %s."), *Owner->GetName(), *Target->GetName());

	ARTSProjectile* SpawnedProjectile = nullptr;

	if (Attack.ProjectileClass != nullptr)
	{
		// Fire projectile.
		// Build spawn transform.
		const FVector SpawnLocation = Owner->GetActorLocation();
		const FRotator SpawnRotation = Owner->GetActorRotation();
		const FTransform SpawnTransform = FTransform(SpawnRotation, SpawnLocation);

		// Build spawn info.
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.Instigator = OwnerPawn;
		SpawnInfo.ObjectFlags |= RF_Transient;

		// Spawn projectile.
		SpawnedProjectile = GetWorld()->SpawnActor<ARTSProjectile>(Attack.ProjectileClass, SpawnTransform, SpawnInfo);

		if (SpawnedProjectile)
		{
			UE_LOG(LogRTS, Log, TEXT("%s fired projectile %s at target %s."), *Owner->GetName(), *SpawnedProjectile->GetName(), *Target->GetName());

			// Aim at target.
			SpawnedProjectile->FireAt(
				Target,
				Damage,
				Attack.DamageType,
				OwnerController,
				Owner);
		}
	}
	else
	{
		// Deal damage immediately.
		Target->TakeDamage(Damage, FDamageEvent(Attack.DamageType), OwnerController, Owner);
	}

	// Start cooldown timer.
	RemainingCooldown = Attack.Cooldown;

	// Notify listeners.
	OnAttackUsed.Broadcast(Owner, Attack, Target, SpawnedProjectile);
}

float URTSAttackComponent::GetAcquisitionRadius() const
{
	return AcquisitionRadius;
}

float URTSAttackComponent::GetChaseRadius() const
{
	return ChaseRadius;
}

TArray<FRTSAttackData> URTSAttackComponent::GetAttacks() const
{
	return Attacks;
}

float URTSAttackComponent::GetRemainingCooldown() const
{
	return RemainingCooldown;
}
