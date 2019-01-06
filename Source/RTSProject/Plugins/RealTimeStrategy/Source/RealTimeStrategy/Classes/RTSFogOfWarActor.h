#pragma once

#include "RealTimeStrategyPCH.h"

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
UCLASS()
class REALTIMESTRATEGY_API ARTSFogOfWarActor : public AActor
{
	GENERATED_BODY()

public:
	ARTSFogOfWarActor(const FObjectInitializer& ObjectInitializer);

	void BeginPlay() override;
	void Tick(float DeltaTime) override;

    /** Gets the texture containing visibility information. */
    UTexture2D* GetFogOfWarTexture() const;

	/** Sets the vision info to render in 3D space. */
	void SetupVisionInfo(ARTSVisionInfo* VisionInfo);

private:
	/** Material to instance for rendering the fog of war effect. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* FogOfWarMaterial;

	/** Renders the fog of war. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	APostProcessVolume* FogOfWarVolume;

	/** Provides visibility information for how to render the fog of war. */
	ARTSVisionInfo* VisionInfo;

	/** Provides world size information for how to render the fog of war. */
	ARTSVisionVolume* VisionVolume;

	/** Texture containing visibility information to be rendered in 3D space. */
	UTexture2D* FogOfWarTexture; 

	/** Buffer for updating the contents of the fog of war texture. */
	uint8* FogOfWarTextureBuffer;

	/** Update region for updating the contents of the fog of war texture. */
	FUpdateTextureRegion2D* FogOfWarUpdateTextureRegion;

	/** Post-process material instance for rendering fog of war in 3D space. */
	UMaterialInstanceDynamic* FogOfWarMaterialInstance;
};
