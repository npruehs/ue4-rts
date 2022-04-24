#pragma once

#include "CoreMinimal.h"

#include "UI/RTSActorWidgetComponent.h"

#include "RTSConstructionProgressBarWidgetComponent.generated.h"


/**
* Adds a construction progress bar widget to the actor.
*/
UCLASS(Category="RTS", Blueprintable)
class REALTIMESTRATEGY_API URTSConstructionProgressBarWidgetComponent : public URTSActorWidgetComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	/** Event when the current construction progress of the actor has changed. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS")
	void UpdateConstructionProgressBar(float ProgressPercentage);


private:
	UFUNCTION()
	void OnConstructionProgressChanged(AActor* Actor, float ProgressPercentage);
};
