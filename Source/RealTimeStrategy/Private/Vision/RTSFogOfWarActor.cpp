#include "Vision/RTSFogOfWarActor.h"

#include "EngineUtils.h"
#include "RHI.h"
#include "Engine/PostProcessVolume.h"
#include "Engine/Texture2D.h"
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"

#include "RTSLog.h"
#include "Vision/RTSVisionInfo.h"
#include "Vision/RTSVisionVolume.h"


ARTSFogOfWarActor::ARTSFogOfWarActor(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
}

void ARTSFogOfWarActor::Initialize(ARTSVisionVolume* InVisionVolume)
{
	// Get vision size.
	VisionVolume = InVisionVolume;

	if (!VisionVolume)
	{
		UE_LOG(LogRTS, Warning, TEXT("No vision volume found, won't update fog of war."));
		return;
	}

	if (!FogOfWarVolume)
	{
		UE_LOG(LogRTS, Warning, TEXT("No fog of war volume found, won't render fog of war."));
		return;
	}

	// Setup fog of war buffer.
	const int32 SizeInTiles = VisionVolume->GetSizeInTiles();
	const FVector SizeInWorld = VisionVolume->GetSizeInWorld();

	FogOfWarTextureBuffer = new uint8[SizeInTiles * SizeInTiles * 4];

	// Setup fog of war texture.
	FogOfWarTexture = UTexture2D::CreateTransient(SizeInTiles, SizeInTiles);

#if WITH_EDITORONLY_DATA
	FogOfWarTexture->MipGenSettings = TextureMipGenSettings::TMGS_NoMipmaps;
#endif

	FogOfWarTexture->AddToRoot();

	FogOfWarTexture->UpdateResource();

	FogOfWarUpdateTextureRegion = new FUpdateTextureRegion2D(0, 0, 0, 0, SizeInTiles, SizeInTiles);

	// Setup fog of war material.
	FogOfWarMaterialInstance = UMaterialInstanceDynamic::Create(FogOfWarMaterial, nullptr);
	FogOfWarMaterialInstance->SetTextureParameterValue(FName("VisibilityMask"), FogOfWarTexture);
	FogOfWarMaterialInstance->SetScalarParameterValue(FName("OneOverWorldSize"), 1.0f / SizeInWorld.X);
	FogOfWarMaterialInstance->SetScalarParameterValue(FName("OneOverTileSize"), 1.0f / SizeInTiles);

	// Setup fog of war post-process volume.
	FogOfWarVolume->AddOrUpdateBlendable(FogOfWarMaterialInstance);

	UE_LOG(LogRTS, Log, TEXT("Set up %s with %s."), *GetName(), *VisionVolume->GetName());
}

void ARTSFogOfWarActor::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);

	// Update texture.
	if (!VisionVolume || !FogOfWarVolume || !VisionInfo)
	{
		return;
	}

	const int32 SizeInTiles = VisionVolume->GetSizeInTiles();

	for (int32 Y = 0; Y < SizeInTiles; ++Y)
	{
		for (int32 X = 0; X < SizeInTiles; ++X)
		{
			const int i = Y * SizeInTiles + X;

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

	FogOfWarTexture->UpdateTextureRegions(0, 1, FogOfWarUpdateTextureRegion, SizeInTiles * 4, (uint32)4, FogOfWarTextureBuffer);
}

UTexture2D* ARTSFogOfWarActor::GetFogOfWarTexture() const
{
	return FogOfWarTexture;
}

void ARTSFogOfWarActor::SetupVisionInfo(ARTSVisionInfo* InVisionInfo)
{
	VisionInfo = InVisionInfo;
}
