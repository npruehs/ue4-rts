#pragma once

#include "RTSPluginPrivatePCH.h"

#include "Components/ActorComponent.h"

#include "RTSSelectableComponent.generated.h"


UCLASS(meta=(BlueprintSpawnableComponent))
class URTSSelectableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URTSSelectableComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
	
};
