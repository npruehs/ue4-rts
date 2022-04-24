#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"

#include "RTSContainableComponent.generated.h"


class AActor;
class AController;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRTSContainableComponentContainerChangedSignature, AActor*, Actor, AActor*, NewContainer);


/** Allows the actor to be contained by other actors. */
UCLASS(Category="RTS", meta = (BlueprintSpawnableComponent))
class REALTIMESTRATEGY_API URTSContainableComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URTSContainableComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;


	/** Gets the actor containing this one. */
	UFUNCTION(BlueprintPure)
	AActor* GetContainer() const;

	/** Sets the actor containing this one. */
	void SetContainer(AActor* NewContainer);

	/** Event when the actor containing the actor has changed. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
	FRTSContainableComponentContainerChangedSignature OnContainerChanged;

private:
	/** Actor containing this one. */
	UPROPERTY(ReplicatedUsing = ReceivedContainer)
	AActor* Container;

	UFUNCTION()
	void ReceivedContainer();

	UFUNCTION()
	void OnKilled(AActor* Actor, AController* PreviousOwner, AActor* DamageCauser);
};
