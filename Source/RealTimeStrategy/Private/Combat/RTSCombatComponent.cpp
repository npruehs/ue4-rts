#include "Combat/RTSCombatComponent.h"
#include "RTSGameMode.h"
#include "RTSLog.h"
#include "RTSPlayerAdvantageComponent.h"
#include "RTSPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Libraries/RTSGameplayLibrary.h"
#include "Libraries/RTSGameplayTagLibrary.h"
#include "Sound/SoundCue.h"

URTSCombatComponent::URTSCombatComponent(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	OnActiveGameplayEffectAddedDelegateToSelf.AddLambda([this](UAbilitySystemComponent* Component, const FGameplayEffectSpec& Spec, FActiveGameplayEffectHandle Handle)
	{
		AActor* Instigator = Spec.GetEffectContext().GetInstigator();

		if (IsValid(Instigator))
		{
			UE_LOG(LogRTS, Log, TEXT("Instigator was %s"), *Instigator->GetActorLabel())
		}
	});
}

void URTSCombatComponent::UseAttack(int32 AttackIndex, AActor* Target)
{
	if (!Attacks.IsValidIndex(AttackIndex))
	{
		UE_LOG(LogRTS, Error, TEXT("There is no AttackData on Index %i"), AttackIndex)
		return;
	}

	const AActor* Owner = GetOwner();

	UE_LOG(LogRTS, Log, TEXT("Actor %s attacks %s."), *Owner->GetName(), *Target->GetName());

	if (!TryActivateAbility(AbilitySpecs[AttackIndex]))
	{
		UE_LOG(LogRTS, Warning, TEXT("Could not activate %s"), *Attacks[AttackIndex].AbilityClass->GetName())
	}
}

void URTSCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	const UAbilitySystemComponent* AbilitySystemComponent = GetOwner()->FindComponentByClass<UAbilitySystemComponent>();
	AttributeSet = Cast<URTSAttributeSet>(AbilitySystemComponent->GetAttributeSet(URTSAttributeSet::StaticClass()));

	if (GetOwner()->GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	for (const FRTSAttackData Attack : Attacks)
	{
		UGameplayAbility* GameplayAbility = Attack.AbilityClass->GetDefaultObject<UGameplayAbility>();
		FGameplayAbilitySpec Ability(
			GameplayAbility,
			0,
			0);

		AbilitySpecs.Emplace(GiveAbility(Ability));
	}
}

void URTSCombatComponent::AddGameplayTags(FGameplayTagContainer& InOutTagContainer)
{
	InOutTagContainer.AddTag(URTSGameplayTagLibrary::Status_Changing_Alive());
	InOutTagContainer.AddTag(URTSGameplayTagLibrary::Status_Permanent_CanAttack());
}

void URTSCombatComponent::KillActor(AActor* DamageCauser)
{
	AActor* Owner = GetOwner();

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
	default: ;
	}

	OwningPlayer->GetPlayerState<ARTSPlayerState>()->Remove(Owner);

	if (IsValid(DeathSound))
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation());
	}

	// Notify game mode.

	if (ARTSGameMode* GameMode = Cast<ARTSGameMode>(UGameplayStatics::GetGameMode(GetWorld())); GameMode != nullptr)
	{
		GameMode->NotifyOnActorKilled(Owner, OwningPlayer);
	}
}
