#include "RTSPluginPrivatePCH.h"
#include "RTSCharacter.h"

#include "Kismet/GameplayStatics.h"

#include "RTSAttackComponent.h"
#include "RTSGameMode.h"
#include "RTSHealthComponent.h"
#include "RTSProjectile.h"


void ARTSCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Cache component references.
	AttackComponent = FindComponentByClass<URTSAttackComponent>();
}

void ARTSCharacter::Tick(float DeltaSeconds)
{
	// Update cooldown timer.
	if (AttackComponent && AttackComponent->RemainingCooldown > 0)
	{
		AttackComponent->RemainingCooldown -= DeltaSeconds;

		if (AttackComponent->RemainingCooldown <= 0)
		{
			UE_LOG(RTSLog, Log, TEXT("Character %s attack is ready again."), *GetName());

			// Notify listeners.
			NotifyOnCooldownReady();
		}
	}
}

float ARTSCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	// Adjust health.
	URTSHealthComponent* HealthComponent = FindComponentByClass<URTSHealthComponent>();

	if (!HealthComponent)
	{
		return 0.0f;
	}

	float OldHealth = HealthComponent->CurrentHealth;
	HealthComponent->CurrentHealth -= Damage;
	float NewHealth = HealthComponent->CurrentHealth;

	UE_LOG(RTSLog, Log, TEXT("Character %s has taken %f damage from %s, reducing health to %f."),
		*GetName(),
		Damage,
		*DamageCauser->GetName(),
		HealthComponent->CurrentHealth);

	// Notify listeners.
	NotifyOnHealthChanged(OldHealth, NewHealth);

	// Check if we've just died.
	if (HealthComponent->CurrentHealth <= 0)
	{
		UE_LOG(RTSLog, Log, TEXT("Character %s has been killed."), *GetName());

		// Get owner before destruction.
		AController* Owner = Cast<AController>(GetOwner());

		// Destroy this actor.
		Destroy();

		// Notify game mode.
		ARTSGameMode* GameMode = Cast<ARTSGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

		if (GameMode != nullptr)
		{
			GameMode->NotifyOnCharacterKilled(this, Owner);
		}

		// Notify listeners.
		NotifyOnKilled(Owner);
	}

	return ActualDamage;
}

void ARTSCharacter::UseAttack(int AttackIndex, AActor* Target)
{
	if (AttackComponent == nullptr)
	{
		return;
	}

	// Check cooldown.
	if (AttackComponent->RemainingCooldown > 0)
	{
		return;
	}

	// Use attack.
	UE_LOG(RTSLog, Log, TEXT("Character %s attacks %s."), *GetName(), *Target->GetName());

	const FRTSAttackData& Attack = AttackComponent->Attacks[0];

	ARTSProjectile* SpawnedProjectile = nullptr;

	if (Attack.ProjectileType != nullptr)
	{
		// Fire projectile.
		// Build spawn transform.
		FVector SpawnLocation = GetActorLocation();
		FRotator SpawnRotation = GetActorRotation();
		FTransform SpawnTransform = FTransform(SpawnRotation, SpawnLocation);

		// Build spawn info.
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.Instigator = this;
		SpawnInfo.ObjectFlags |= RF_Transient;

		// Spawn projectile.
		SpawnedProjectile = GetWorld()->SpawnActor<ARTSProjectile>(Attack.ProjectileType, SpawnTransform, SpawnInfo);

		if (SpawnedProjectile)
		{
			UE_LOG(RTSLog, Log, TEXT("%s fired projectile %s at target %s."), *GetName(), *SpawnedProjectile->GetName(), *Target->GetName());

			// Aim at target.
			SpawnedProjectile->FireAt(Target, Attack.Damage, Attack.DamageType, GetController(), this);
		}
	}
	else
	{
		// Deal damage immediately.
		Target->TakeDamage(Attack.Damage, FDamageEvent(Attack.DamageType), GetController(), this);
	}

	// Start cooldown timer.
	AttackComponent->RemainingCooldown = Attack.Cooldown;

	// Notify listeners.
	NotifyOnUsedAttack(Attack, Target, SpawnedProjectile);
}

void ARTSCharacter::NotifyOnCooldownReady()
{
	ReceiveOnCooldownReady();
}

void ARTSCharacter::NotifyOnHealthChanged(float OldHealth, float NewHealth)
{
	ReceiveOnHealthChanged(OldHealth, NewHealth);
}

void ARTSCharacter::NotifyOnKilled(AController* PreviousOwner)
{
	ReceiveOnKilled(PreviousOwner);
}

void ARTSCharacter::NotifyOnOwnerChanged(AController* NewOwner)
{
	ReceiveOnOwnerChanged(NewOwner);
}

void ARTSCharacter::NotifyOnUsedAttack(const FRTSAttackData& Attack, AActor* Target, ARTSProjectile* Projectile)
{
	ReceiveOnUsedAttack(Attack, Target, Projectile);
}
