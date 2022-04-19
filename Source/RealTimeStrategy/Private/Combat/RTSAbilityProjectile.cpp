// Fill out your copyright notice in the Description page of Project Settings.

#include "Combat/RTSAbilityProjectile.h"

#include "AbilitySystemComponent.h"
#include "Combat/RTSAttributeSet.h"
#include "RTSLog.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameplayAbilities/Public/AbilitySystemGlobals.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundBase.h"

ARTSAbilityProjectile::ARTSAbilityProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	ProjectileMesh->SetCollisionResponseToAllChannels(ECR_Block);
	ProjectileMesh->OnComponentHit.AddDynamic(this, &ARTSAbilityProjectile::OnHit);
	ProjectileMesh->OnComponentBeginOverlap.AddDynamic(this, &ARTSAbilityProjectile::OnOverlap);

	RootComponent = ProjectileMesh;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("Projectile Ability System Component"));
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
}

void ARTSAbilityProjectile::SetTarget(const AActor* Target)
{
	ProjectileMovement->HomingTargetComponent = Target->GetRootComponent();

	TargetLocation = Target->GetActorLocation();
	TravelDistance = FVector::Distance(StartLocation, Target->GetActorLocation());
}

void ARTSAbilityProjectile::SetOwningActor(const AActor* InOwner)
{
	OwningActor = InOwner;
}

void ARTSAbilityProjectile::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetActorLocation();

	if (IsValid(LaunchSound))
	{
		UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, GetActorLocation());
	}

	AttributeSet = AbilitySystemComponent->GetSet<URTSAttributeSet>();
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	ProjectileMesh->SetCollisionResponseToAllChannels(bCanBeBodyBlocked ? ECR_Block : ECR_Overlap);
}

float ARTSAbilityProjectile::GetDistanceToTarget() const
{
	if (ProjectileMovement->bIsHomingProjectile && IsValid(ProjectileMovement->HomingTargetComponent.Get()))
	{
		return FVector::Distance(ProjectileMovement->HomingTargetComponent->GetComponentToWorld().GetLocation(), GetActorLocation());
	}

	return FVector::Distance(TargetLocation, GetActorLocation());
}

void ARTSAbilityProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult)
{
	if (!bCanBeBodyBlocked || OtherActor == GetOwner())
	{
		return;
	}

	if (OtherActor == this)
	{
		return;
	}

	CauseDamage(OtherActor);
	Destroy();
}

void ARTSAbilityProjectile::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bCanBeBodyBlocked)
	{
		return;
	}

	if (OtherActor == this)
	{
		return;
	}

	if (OtherComp != ProjectileMovement->HomingTargetComponent.Get())

	{
		return;
	}

	CauseDamage(OtherActor);
	Destroy();
}

void ARTSAbilityProjectile::CauseDamage(const AActor* Target) const
{
	if (OwningActor == Target)
	{
		return;
	}

	if (!IsValid(EffectClass))
	{
		UE_LOG(LogRTS, Error, TEXT("You need to define an effect-class for %s"), *GetName())
		return;
	}

	if (IsValid(HitParticle))
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, HitParticle, GetActorLocation(), FRotator(0, 0, 0));
	}

	if (IsValid(HitSound))
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
	}

	if (UAbilitySystemComponent* AbilitySystemComponentFromActor = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Target); IsValid(AbilitySystemComponentFromActor) && IsValid(AbilitySystemComponent))
	{
		const FGameplayEffectContextHandle Context;
		UGameplayEffect* Effect = EffectClass->GetDefaultObject<UGameplayEffect>();
		AbilitySystemComponent->ApplyGameplayEffectToTarget(Effect, AbilitySystemComponentFromActor, 1, Context);
	}
}
