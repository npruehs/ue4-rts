#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Volume.h"

#include "RTSMinimapVolume.generated.h"


class UTexture2D;


/**
* Volume that defines the size and resolution of the minimap.
*/
UCLASS(Category="RTS", Blueprintable)
class REALTIMESTRATEGY_API ARTSMinimapVolume : public AVolume
{
	GENERATED_BODY()

public:
	explicit ARTSMinimapVolume(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

#if WITH_EDITOR
	virtual void PostEditMove(bool bFinished) override;
#endif
	
	/** Gets the Background image of the minimap. */
	UFUNCTION(BlueprintPure)
	UTexture* GetMinimapImage() const;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void GenerateSnapshot();

protected:
	// Actors of these classes are hidden from the generated background
	UPROPERTY(EditAnywhere, Category = "RTS|Generation")
	TArray<TSubclassOf<AActor>> HiddenActorClasses;
	// These actors in the level are hidden from the generated backgrounds
	UPROPERTY(EditAnywhere, Category = "RTS|Generation")
	TArray<AActor*> HiddenActors;

private:
	/** Background image of the minimap. */
	UPROPERTY(EditInstanceOnly, Category = "RTS")
	UTexture2D* MinimapImage = nullptr;

	// Used to capture the level if no MinimapImage is set
	UPROPERTY(VisibleAnywhere, Category = "RTS|Generation")
	USceneCaptureComponent2D* CaptureComponent2D;
};
