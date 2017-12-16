#pragma once

#include "RTSPluginPCH.h"

#include "Components/ActorComponent.h"

#include "RTSContainerComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRTSContainerComponentActorEnteredSignature, AActor*, Actor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRTSContainerComponentActorLeftSignature, AActor*, Actor);


/** Can hold one or more actors. */
UCLASS(meta = (BlueprintSpawnableComponent))
class URTSContainerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/** Actors held by this container. */
	TArray<AActor*> ContainedActors;

	/** How many actors may enter this container at the same time. Negative number means unlimited capacity, or will be set elsewhere. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS")
	int32 Capacity;


	URTSContainerComponent(const FObjectInitializer& ObjectInitializer);


	/** Whether the specified actor can enter this container. */
	UFUNCTION(BlueprintCallable)
	virtual bool CanLoadActor(AActor* Actor) const;

	/** Adds the specified actor to this container. */
	UFUNCTION(BlueprintCallable)
	virtual void LoadActor(AActor* Actor);

	/** Remvoes the specified actor from this container. */
	UFUNCTION(BlueprintCallable)
	virtual void UnloadActor(AActor* Actor);


	/** Event when a new actor has entered this container. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
	FRTSContainerComponentActorEnteredSignature OnActorEntered;

	/** Event when an actor has left this container. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
	FRTSContainerComponentActorLeftSignature OnActorLeft;
};
