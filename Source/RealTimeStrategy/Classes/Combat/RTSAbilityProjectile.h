// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "RTSAbilityProjectile.generated.h"

class UGameplayEffect;
class URTSAttributeSet;
class USphereComponent;
class UProjectileMovementComponent;
/**
 * 
 */
UCLASS()
class REALTIMESTRATEGY_API ARTSAbilityProjectile : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ARTSAbilityProjectile();


	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }

	UFUNCTION(BlueprintCallable)
	void SetTarget(const AActor* Target);
	UFUNCTION(BlueprintCallable)
	void SetOwningActor(const AActor* InOwner);

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintPure)
	float GetDistanceToTarget() const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USphereComponent* Sphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Abilities")
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Abilities")
	const URTSAttributeSet* AttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Abilities")
	TSubclassOf<UGameplayEffect> EffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Effects")
	UParticleSystem* HitParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Effects")
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Effects")
	USoundBase* LaunchSound;

	UPROPERTY()
	const AActor* OwningActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float TravelDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanBeBodyBlocked;

	UPROPERTY()
	FVector StartLocation;

	UPROPERTY()
	FVector TargetLocation;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult);

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	void CauseDamage(const AActor* Target) const;
};
