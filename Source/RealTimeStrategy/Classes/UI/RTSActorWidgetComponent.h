#pragma once

#include "CoreMinimal.h"

#include "Components/WidgetComponent.h"

#include "RTSActorWidgetComponent.generated.h"


/**
* Adds a customizable UI widget to the actor.
*/
UCLASS()
class REALTIMESTRATEGY_API URTSActorWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	URTSActorWidgetComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Event when the size of the actor on screen been calculated from its collision size. */
	UFUNCTION(BlueprintNativeEvent, Category = "RTS")
	void UpdatePositionAndSize(const FVector2D& ActorScreenSize);
};
