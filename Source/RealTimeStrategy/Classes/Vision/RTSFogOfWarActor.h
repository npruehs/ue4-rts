#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "RTSFogOfWarActor.generated.h"


class UTexture2D;
struct FUpdateTextureRegion2D;
class UMaterialInstanceDynamic;
class UMaterialInterface;
class APostProcessVolume;

class ARTSVisionInfo;
class ARTSVisionVolume;


/** Renders fog of war in 3D space. */
UCLASS(Category="RTS")
class REALTIMESTRATEGY_API ARTSFogOfWarActor : public AActor
{
	GENERATED_BODY()

public:
	ARTSFogOfWarActor(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** Prepares this actor for gameplay. */
	void Initialize(ARTSVisionVolume* InVisionVolume);

	virtual void Tick(float DeltaTime) override;

	/** Gets the texture containing visibility information. */
	UTexture2D* GetFogOfWarTexture() const;

	/** Sets the vision info to render in 3D space. */
	void SetupVisionInfo(ARTSVisionInfo* VisionInfo);

private:
	/** Renders the fog of war. */
	UPROPERTY(EditInstanceOnly, Category = "RTS")
	APostProcessVolume* FogOfWarVolume;

	/** Material to instance for rendering the fog of war effect. */
	UPROPERTY(EditInstanceOnly, Category = "RTS")
	UMaterialInterface* FogOfWarMaterial;

	/** Provides visibility information for how to render the fog of war. */
	UPROPERTY()
	ARTSVisionInfo* VisionInfo;

	/** Provides world size information for how to render the fog of war. */
	UPROPERTY()
	ARTSVisionVolume* VisionVolume;

	/** Texture containing visibility information to be rendered in 3D space. */
	UPROPERTY()
	UTexture2D* FogOfWarTexture;

	/** Buffer for updating the contents of the fog of war texture. */
	uint8* FogOfWarTextureBuffer;

	/** Update region for updating the contents of the fog of war texture. */
	FUpdateTextureRegion2D* FogOfWarUpdateTextureRegion;

	/** Post-process material instance for rendering fog of war in 3D space. */
	UPROPERTY()
	UMaterialInstanceDynamic* FogOfWarMaterialInstance;
};
