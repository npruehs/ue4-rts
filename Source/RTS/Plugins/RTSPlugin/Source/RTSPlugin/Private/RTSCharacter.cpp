#include "RTSPluginPrivatePCH.h"
#include "RTSCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

#include "RTSGameMode.h"
#include "RTSHealthComponent.h"
#include "RTSProjectile.h"



ARTSCharacter::ARTSCharacter()
{
	// Create selection circle.
	SelectionCircleDecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("SelectionCircleDecal"));
	SelectionCircleDecalComponent->SetRelativeRotation(FRotator::MakeFromEuler(FVector(0.0f, -90.0f, 0.0f)));

	// Enable replication.
	bReplicates = true;
}

void ARTSCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Cache component references.
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

void ARTSCharacter::NotifyOnSelected()
{
	bSelected = true;

	// Notify listeners.
	ReceiveOnSelected();
}
