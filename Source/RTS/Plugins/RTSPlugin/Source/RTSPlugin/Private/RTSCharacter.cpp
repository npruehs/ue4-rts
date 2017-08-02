#include "RTSPluginPrivatePCH.h"
#include "RTSCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

#include "RTSAttackComponent.h"
#include "RTSGameMode.h"
#include "RTSHealthComponent.h"
#include "RTSPlayerState.h"
#include "RTSProjectile.h"
#include "RTSTeamInfo.h"


ARTSCharacter::ARTSCharacter()
{
	// Create selection circle.
	SelectionCircleDecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("SelectionCircleDecal"));
	SelectionCircleDecalComponent->SetRelativeRotation(FRotator::MakeFromEuler(FVector(0.0f, -90.0f, 0.0f)));

	// Enable replication.
	bReplicates = true;
}

void ARTSCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARTSCharacter, PlayerOwner);
}

ARTSPlayerState* ARTSCharacter::GetPlayerOwner()
{
	return PlayerOwner;
}

void ARTSCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Cache component references.
	AttackComponent = FindComponentByClass<URTSAttackComponent>();
	HealthComponent = FindComponentByClass<URTSHealthComponent>();

	// Setup selection circle.
	FCollisionShape CollisionShape = GetCapsuleComponent()->GetCollisionShape();
	float DecalHeight = CollisionShape.Capsule.HalfHeight * 2;
	float DecalRadius = CollisionShape.Capsule.Radius * 2;

	SelectionCircleDecalComponent->DecalSize = FVector(DecalHeight, DecalRadius, DecalRadius);


	if (HealthComponent)
	{
		HealthComponent->SetIsReplicated(true);
	}
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

	if (bSelected)
	{
		// Show selection circle.
		SelectionCircleDecalComponent->SetWorldLocation(GetActorLocation());
	}
	else
	{
		// HACK(np): Hide selection circle.
		SelectionCircleDecalComponent->SetWorldLocation(FVector(0.0f, 0.0f, -10000.0f));
	}
}

float ARTSCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	// Adjust health.
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
		AController* OwningPlayer = Cast<AController>(GetOwner());

		// Destroy this actor.
		Destroy();

		// Notify game mode.
		ARTSGameMode* GameMode = Cast<ARTSGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

		if (GameMode != nullptr)
		{
			GameMode->NotifyOnCharacterKilled(this, OwningPlayer);
		}

		// Notify listeners.
		NotifyOnKilled(OwningPlayer);
	}

	return ActualDamage;
}

bool ARTSCharacter::IsSameTeamAsCharacter(ARTSCharacter* Other)
{
	ARTSPlayerState* MyOwner = GetPlayerOwner();
	ARTSPlayerState* OtherOwner = Other->GetPlayerOwner();

	return MyOwner && MyOwner->IsSameTeamAs(OtherOwner);
}

bool ARTSCharacter::IsSameTeamAsController(AController* C)
{
	ARTSPlayerState* MyOwner = GetPlayerOwner();
	ARTSPlayerState* OtherPlayer = Cast<ARTSPlayerState>(C->PlayerState);

	return MyOwner && MyOwner->IsSameTeamAs(OtherPlayer);
}

void ARTSCharacter::UseAttack(int AttackIndex, AActor* Target)
{
	if (AttackComponent == nullptr)
	{
		return;
	}

	if (!IsValid(Target))
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

void ARTSCharacter::NotifyOnDeselected()
{
	bSelected = false;

	// Notify listeners.
	ReceiveOnDeselected();
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
	PlayerOwner = Cast<ARTSPlayerState>(NewOwner->PlayerState);

	if (PlayerOwner)
	{
		ReceiveOnOwnerChanged(NewOwner);
	}
}

void ARTSCharacter::NotifyOnSelected()
{
	bSelected = true;

	// Notify listeners.
	ReceiveOnSelected();
}

void ARTSCharacter::NotifyOnUsedAttack(const FRTSAttackData& Attack, AActor* Target, ARTSProjectile* Projectile)
{
	ReceiveOnUsedAttack(Attack, Target, Projectile);
}
