#pragma once

#include "CoreMinimal.h"

#include "UI/RTSActorWidgetComponent.h"

#include "RTSProductionProgressBarWidgetComponent.generated.h"


/**
* Adds a production progress bar widget to the actor.
*/
UCLASS(Category="RTS", Blueprintable)
class REALTIMESTRATEGY_API URTSProductionProgressBarWidgetComponent : public URTSActorWidgetComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	/** Event when the current production progress of the actor has changed. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS")
	void UpdateProductionProgressBar(int32 QueueIndex, float ProgressPercentage);


private:
	UFUNCTION()
	void OnProductionProgressChanged(AActor* Actor, int32 QueueIndex, float ProgressPercentage);
};
