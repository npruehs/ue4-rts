#include "Combat/RTSHealthComponent.h"

#include "TimerManager.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Sound/SoundCue.h"

#include "RTSGameMode.h"
#include "RTSGameplayTagsComponent.h"
#include "RTSLog.h"
#include "Libraries/RTSGameplayLibrary.h"
#include "Libraries/RTSGameplayTagLibrary.h"


URTSHealthComponent::URTSHealthComponent(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(true);

	// Set reasonable default values.
	MaximumHealth = 100.0f;
	bRegenerateHealth = false;
	ActorDeathType = ERTSActorDeathType::DEATH_Destroy;

	InitialGameplayTags.AddTag(URTSGameplayTagLibrary::Status_Changing_Alive());
}

void URTSHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URTSHealthComponent, CurrentHealth);
}

void URTSHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// Set initial health.
	CurrentHealth = MaximumHealth;

	// Register for events.
	AActor* Owner = GetOwner();

	if (!IsValid(Owner))
	{
		return;
	}

	Owner->OnTakeAnyDamage.AddDynamic(this, &URTSHealthComponent::OnTakeAnyDamage);

	if (bRegenerateHealth && Owner->HasAuthority())
	{
		// Set up health regeneration timer.
		Owner->GetWorldTimerManager().SetTimer(
			HealthRegenerationTimer, this, &URTSHealthComponent::OnHealthRegenerationTimerElapsed, 1.0f, true);
	}

	LastTimeDamageTaken = 0.0f;
}

float URTSHealthComponent::GetMaximumHealth() const
{
	return MaximumHealth;
}

float URTSHealthComponent::GetCurrentHealth() const
{
	return CurrentHealth;
}

void URTSHealthComponent::SetCurrentHealth(float NewHealth, AActor* DamageCauser)
{
	const float OldHealth = CurrentHealth;
	CurrentHealth = NewHealth;

	// Notify listeners.
	AActor* Owner = GetOwner();

	NotifyOnHealthChanged(Owner, OldHealth, NewHealth, DamageCauser);

	if (GetWorld() && OldHealth > NewHealth)
	{
		LastTimeDamageTaken = GetWorld()->GetRealTimeSeconds();
	}

	// Check if we've just died.
	if (CurrentHealth <= 0)
	{
		UE_LOG(LogRTS, Log, TEXT("Actor %s has been killed."), *Owner->GetName());

		// Get owner before destruction.
		AController* OwningPlayer = Cast<AController>(Owner->GetOwner());

		// Remove Alive tag.
		URTSGameplayTagLibrary::RemoveGameplayTag(Owner, URTSGameplayTagLibrary::Status_Changing_Alive());

		// Notify listeners.
		OnKilled.Broadcast(Owner, OwningPlayer, DamageCauser);

		// Stop or destroy actor.
		switch (ActorDeathType)
		{
		case ERTSActorDeathType::DEATH_StopGameplay:
			URTSGameplayLibrary::StopGameplayFor(Owner);
			break;

		case ERTSActorDeathType::DEATH_Destroy:
			Owner->Destroy();
			break;
		}

		// Notify game mode.
		ARTSGameMode* GameMode = Cast<ARTSGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

		if (GameMode != nullptr)
		{
			GameMode->NotifyOnActorKilled(Owner, OwningPlayer);
		}
	}
}

void URTSHealthComponent::KillActor(AActor* DamageCauser /*= nullptr*/)
{
	SetCurrentHealth(0.0f, DamageCauser);
}

float URTSHealthComponent::GetLastTimeDamageTaken() const
{
	return LastTimeDamageTaken;
}

void URTSHealthComponent::NotifyOnHealthChanged(AActor* Actor, float OldHealth, float NewHealth, AActor* DamageCauser)
{
	// Notify listeners.
	OnHealthChanged.Broadcast(Actor, OldHealth, NewHealth, DamageCauser);

	// Play sound.
	if (NewHealth <= 0.0f && IsValid(DeathSound))
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound,
		                                      Actor->GetActorLocation(), Actor->GetActorRotation());
	}
}

void URTSHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	SetCurrentHealth(CurrentHealth - Damage, DamageCauser);
}

void URTSHealthComponent::OnHealthRegenerationTimerElapsed()
{
	if (CurrentHealth >= MaximumHealth)
	{
		return;
	}

	const float NewHealth = FMath::Clamp(CurrentHealth + HealthRegenerationRate, 0.0f, MaximumHealth);
	SetCurrentHealth(NewHealth, GetOwner());
}

void URTSHealthComponent::ReceivedCurrentHealth(float OldHealth)
{
	NotifyOnHealthChanged(GetOwner(), OldHealth, CurrentHealth, nullptr);
}
