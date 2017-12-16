#pragma once

#include "RTSPluginPCH.h"

#include "GameFramework/Actor.h"

#include "RTSFogOfWarActor.generated.h"


class UTexture2D;
struct FUpdateTextureRegion2D;
class UDynamicMaterialInstance;
class UMaterialInterface;
class APostProcessVolume;

class ARTSVisionInfo;
class ARTSVisionVolume;


/** Renders fog of war in 3D space. */
UCLASS()
class ARTSFogOfWarActor : public AActor
{
	GENERATED_BODY()

public:
	ARTSFogOfWarActor(const FObjectInitializer& ObjectInitializer);

	void BeginPlay() override;
	void Tick(float DeltaTime) override;

	/** Sets the vision info to render in 3D space. */
	void SetupVisionInfo(ARTSVisionInfo* VisionInfo);

	void UpdateTextureRegions(UTexture2D* Texture, int32 MipIndex, uint32 NumRegions, FUpdateTextureRegion2D* Regions, uint32 SrcPitch, uint32 SrcBpp, uint8* SrcData, bool bFreeData);

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
