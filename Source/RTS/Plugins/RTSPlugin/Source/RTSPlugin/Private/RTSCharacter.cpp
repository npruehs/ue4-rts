#include "RTSPluginPrivatePCH.h"
#include "RTSCharacter.h"

#include "RTSAttackComponent.h"
#include "RTSHealthComponent.h"


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

	HealthComponent->CurrentHealth -= Damage;
	
	UE_LOG(RTSLog, Log, TEXT("Character %s has taken %f damage from %s, reducing health to %f."),
		*GetName(),
		Damage,
		*DamageCauser->GetName(),
		HealthComponent->CurrentHealth);

	// Check if we've just died.
	if (HealthComponent->CurrentHealth <= 0)
	{
		Destroy();
		UE_LOG(RTSLog, Log, TEXT("Character %s has been killed."), *GetName());
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
	const FRTSAttackData& Attack = AttackComponent->Attacks[0];
	Target->TakeDamage(Attack.Damage, FDamageEvent(Attack.DamageType), GetController(), this);

	UE_LOG(RTSLog, Log, TEXT("Actor %s attacked %s."), *GetName(), *Target->GetName());

	// Start cooldown timer.
	AttackComponent->RemainingCooldown = Attack.Cooldown;

	// Notify listeners.
	NotifyOnUsedAttack(Attack, Target);
}

void ARTSCharacter::NotifyOnUsedAttack(const FRTSAttackData& Attack, AActor* Target)
{
	ReceiveOnUsedAttack(Attack, Target);
}
