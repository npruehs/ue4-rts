#include "RTSPluginPCH.h"
#include "RTSCharacter.h"

#include "WorldCollision.h"
#include "Components/CapsuleComponent.h"
#include "Components/DecalComponent.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

#include "RTSAttackData.h"
#include "RTSGameMode.h"
#include "RTSHealthComponent.h"
#include "RTSSelectableComponent.h"


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
	SelectableComponent = FindComponentByClass<URTSSelectableComponent>();

	// Setup selection circle.
	FCollisionShape CollisionShape = GetCapsuleComponent()->GetCollisionShape();
	float DecalHeight = CollisionShape.Capsule.HalfHeight * 2;
	float DecalRadius = CollisionShape.Capsule.Radius;

	SelectionCircleDecalComponent->DecalSize = FVector(DecalHeight, DecalRadius, DecalRadius);
}

void ARTSCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (SelectableComponent && SelectableComponent->IsSelected())
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

	return HealthComponent->TakeDamage(ActualDamage, DamageEvent, EventInstigator, DamageCauser);
}
