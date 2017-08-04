#include "RTSPluginPrivatePCH.h"
#include "RTSCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

#include "RTSGameMode.h"
#include "RTSHealthComponent.h"


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

	// Setup selection circle.
	FCollisionShape CollisionShape = GetCapsuleComponent()->GetCollisionShape();
	float DecalHeight = CollisionShape.Capsule.HalfHeight * 2;
	float DecalRadius = CollisionShape.Capsule.Radius * 2;

	SelectionCircleDecalComponent->DecalSize = FVector(DecalHeight, DecalRadius, DecalRadius);
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
	URTSHealthComponent* HealthComponent = FindComponentByClass<URTSHealthComponent>();
	if (!HealthComponent)
	{
		return 0.0f;
	}

	return HealthComponent->TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
}

void ARTSCharacter::NotifyOnDeselected()
{
	bSelected = false;

	// Notify listeners.
	ReceiveOnDeselected();
}

void ARTSCharacter::NotifyOnSelected()
{
	bSelected = true;

	// Notify listeners.
	ReceiveOnSelected();
}
