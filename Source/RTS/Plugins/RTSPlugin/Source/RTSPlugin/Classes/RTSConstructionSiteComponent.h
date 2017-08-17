#pragma once

#include "RTSPluginPrivatePCH.h"

#include "Components/ActorComponent.h"

#include "RTSConstructionState.h"
#include "RTSProductionCostType.h"

#include "RTSConstructionSiteComponent.generated.h"


class URTSResourceType;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRTSConstructionSiteComponentConstructionStartedSignature, float, TotalConstructionTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRTSConstructionSiteComponentConstructionFinishedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRTSConstructionSiteComponentConstructionCanceledSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRTSConstructionSiteComponentBuilderConsumedSignature, AActor*, Builder);


/** Allows constructing the actor over time. */
UCLASS(meta = (BlueprintSpawnableComponent))
class URTSConstructionSiteComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/** Builders currently working at this construction site. */
	TArray<AActor*> AssignedBuilders;

	/** When to pay resources for construction. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS")
	ERTSProductionCostType ConstructionCostType;

	/** Resources to pay for constructing the actor. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS")
	TMap<TSubclassOf<URTSResourceType>, float> ConstructionCosts;

	/** Time for constructing the actor, in seconds. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS")
	float ConstructionTime;

	/** Whether any builders working at this construction site are destroyed when finished. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS")
	bool bConsumesBuilders;

	/** How many builders may be assigned to this construction site at the same time. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS")
	int32 MaxAssignedBuilders;

	/** Factor to multiply all passed construction time with, independent of any currently assigned builders. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS")
	float ProgressMadeAutomatically;

	/** Factor to multiply all passed construction time with, multiplied with the number of currently assigned builders. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS")
	float ProgressMadePerBuilder;

	/** Time before the actor is constructed, in seconds. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS", replicated)
	float RemainingConstructionTime;

	/** Whether to start construction immediately after spawn, or not. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS")
	bool bStartImmediately;


	URTSConstructionSiteComponent(const FObjectInitializer& ObjectInitializer);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;


	/** Whether the specified builder can be assigned to this construction site. */
	UFUNCTION(BlueprintCallable)
	virtual bool CanAssignBuilder(AActor* Builder) const;

	/** Gets the current construction progress [0..1]. */
	UFUNCTION(BlueprintCallable)
	float GetProgressPercentage() const;

	/** Whether the construction timer is currently being ticked, or not. */
	UFUNCTION(BlueprintCallable)
	bool IsConstructing() const;

	/** Whether the construction is finished and the actor ready to use. */
	UFUNCTION(BlueprintCallable)
	bool IsFinished() const;

	/** Starts constructing the actor, setting the timer. */
	UFUNCTION(BlueprintCallable)
	virtual void StartConstruction();

	/** Finishes constructing the actor. */
	UFUNCTION(BlueprintCallable)
	virtual void FinishConstruction();

	/** Cancels constructing the actor, destroying it. */
	UFUNCTION(BlueprintCallable)
	virtual void CancelConstruction();


	/** Event when the construction timer has been started. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
	FRTSConstructionSiteComponentConstructionStartedSignature OnConstructionStarted;

	/** Event when the construction timer has been expired. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
	FRTSConstructionSiteComponentConstructionFinishedSignature OnConstructionFinished;

	/** Event when the construction has been canceled and the construction site destroyed. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
	FRTSConstructionSiteComponentConstructionCanceledSignature OnConstructionCanceled;

	/** Event when a builder working at this construction site has been destroyed. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
	FRTSConstructionSiteComponentBuilderConsumedSignature OnBuilderConsumed;


private:
	/** Whether the construction timer is currently being ticked, or not. */
	ERTSConstructionState State;
};
