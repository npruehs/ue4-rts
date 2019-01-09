#include "RealTimeStrategyPCH.h"
#include "Vision/RTSFogOfWarActor.h"

#include "EngineUtils.h"
#include "RHI.h"
#include "Engine/PostProcessVolume.h"
#include "Engine/Texture2D.h"
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"

#include "Vision/RTSVisionInfo.h"
#include "Vision/RTSVisionVolume.h"


ARTSFogOfWarActor::ARTSFogOfWarActor(const FObjectInitializer& ObjectInitializer)
	: AActor(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
}

void ARTSFogOfWarActor::BeginPlay()
{
	Super::BeginPlay();

	// Get vision size.
	for (TActorIterator<ARTSVisionVolume> It(GetWorld()); It; ++It)
	{
		VisionVolume = *It;
		break;
	}

	if (!VisionVolume)
	{
		UE_LOG(LogRTS, Warning, TEXT("No vision volume found, won't update vision."));
		return;
	}

	if (!FogOfWarVolume)
	{
		UE_LOG(LogRTS, Warning, TEXT("No fog of war volume found, won't render vision."));
		return;
	}

	// Setup fog of war buffer.
	FIntVector TileSize = VisionVolume->GetTileSize();
	FVector WorldSize = VisionVolume->GetWorldSize();

	FogOfWarTextureBuffer = new uint8[TileSize.X * TileSize.Y * 4];

	// Setup fog of war texture.
	FogOfWarTexture = UTexture2D::CreateTransient(TileSize.X, TileSize.Y);

#if WITH_EDITORONLY_DATA
	FogOfWarTexture->MipGenSettings = TextureMipGenSettings::TMGS_NoMipmaps;
#endif

	FogOfWarTexture->AddToRoot();

	FogOfWarTexture->UpdateResource();

	FogOfWarUpdateTextureRegion = new FUpdateTextureRegion2D(0, 0, 0, 0, TileSize.X, TileSize.Y);

	// Setup fog of war material.
	FogOfWarMaterialInstance = UMaterialInstanceDynamic::Create(FogOfWarMaterial, nullptr);
	FogOfWarMaterialInstance->SetTextureParameterValue(FName("VisibilityMask"), FogOfWarTexture);
	FogOfWarMaterialInstance->SetScalarParameterValue(FName("OneOverWorldSize"), 1.0f / WorldSize.X);
	FogOfWarMaterialInstance->SetScalarParameterValue(FName("OneOverTileSize"), 1.0f / TileSize.X);

	// Setup fog of war post-process volume.
	FogOfWarVolume->AddOrUpdateBlendable(FogOfWarMaterialInstance);
}

void ARTSFogOfWarActor::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);

	// Update texture.
	if (!VisionInfo)
	{
		return;
	}

	FIntVector TileSize = VisionVolume->GetTileSize();

	for (int32 Y = 0; Y < TileSize.Y; ++Y)
	{
		for (int32 X = 0; X < TileSize.X; ++X)
		{
			const int i = Y * TileSize.X + X;

			const int iBlue = i * 4 + 0;
			const int iGreen = i * 4 + 1;
			const int iRed = i * 4 + 2;
			const int iAlpha = i * 4 + 3;

			switch (VisionInfo->GetVision(X, Y))
			{
			case ERTSVisionState::VISION_Visible:
				FogOfWarTextureBuffer[iBlue] = 0;
				FogOfWarTextureBuffer[iGreen] = 0;
				FogOfWarTextureBuffer[iRed] = 255;
				FogOfWarTextureBuffer[iAlpha] = 0;
				break;

			case ERTSVisionState::VISION_Known:
				FogOfWarTextureBuffer[iBlue] = 0;
				FogOfWarTextureBuffer[iGreen] = 255;
				FogOfWarTextureBuffer[iRed] = 0;
				FogOfWarTextureBuffer[iAlpha] = 0;
				break;

			case ERTSVisionState::VISION_Unknown:
				FogOfWarTextureBuffer[iBlue] = 0;
				FogOfWarTextureBuffer[iGreen] = 0;
				FogOfWarTextureBuffer[iRed] = 0;
				FogOfWarTextureBuffer[iAlpha] = 0;
				break;
			}
		}
	}

	FogOfWarTexture->UpdateTextureRegions(0, 1, FogOfWarUpdateTextureRegion, TileSize.X * 4, (uint32)4, FogOfWarTextureBuffer);
}

UTexture2D* ARTSFogOfWarActor::GetFogOfWarTexture() const
{
    return FogOfWarTexture;
}

void ARTSFogOfWarActor::SetupVisionInfo(ARTSVisionInfo* InVisionInfo)
{
	VisionInfo = InVisionInfo;
}
