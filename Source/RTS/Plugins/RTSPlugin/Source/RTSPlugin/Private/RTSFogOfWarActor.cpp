#include "RTSPluginPCH.h"
#include "RTSFogOfWarActor.h"

#include "EngineUtils.h"
#include "RHI.h"
#include "Engine/PostProcessVolume.h"
#include "Engine/Texture2D.h"
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"

#include "RTSVisionInfo.h"
#include "RTSVisionVolume.h"


ARTSFogOfWarActor::ARTSFogOfWarActor(const FObjectInitializer& ObjectInitializer)
	: AActor(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
}

void ARTSFogOfWarActor::BeginPlay()
{
	AActor::BeginPlay();

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

	UpdateTextureRegions(FogOfWarTexture, 0, 1, FogOfWarUpdateTextureRegion, TileSize.X * 4, (uint32)4, FogOfWarTextureBuffer, false);
}

UTexture2D* ARTSFogOfWarActor::GetFogOfWarTexture() const
{
    return FogOfWarTexture;
}

void ARTSFogOfWarActor::SetupVisionInfo(ARTSVisionInfo* InVisionInfo)
{
	VisionInfo = InVisionInfo;
}

void ARTSFogOfWarActor::UpdateTextureRegions(UTexture2D* Texture, int32 MipIndex, uint32 NumRegions, FUpdateTextureRegion2D* Regions, uint32 SrcPitch, uint32 SrcBpp, uint8* SrcData, bool bFreeData)
{
	if (Texture->Resource)
	{
		struct FUpdateTextureRegionsData
		{
			FTexture2DResource* Texture2DResource;
			int32 MipIndex;
			uint32 NumRegions;
			FUpdateTextureRegion2D* Regions;
			uint32 SrcPitch;
			uint32 SrcBpp;
			uint8* SrcData;
		};

		FUpdateTextureRegionsData* RegionData = new FUpdateTextureRegionsData;

		RegionData->Texture2DResource = (FTexture2DResource*)Texture->Resource;
		RegionData->MipIndex = MipIndex;
		RegionData->NumRegions = NumRegions;
		RegionData->Regions = Regions;
		RegionData->SrcPitch = SrcPitch;
		RegionData->SrcBpp = SrcBpp;
		RegionData->SrcData = SrcData;

		ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(
			UpdateTextureRegionsData,
			FUpdateTextureRegionsData*, RegionData, RegionData,
			bool, bFreeData, bFreeData,
			{
				for (uint32 RegionIndex = 0; RegionIndex < RegionData->NumRegions; ++RegionIndex)
				{
					int32 CurrentFirstMip = RegionData->Texture2DResource->GetCurrentFirstMip();
					if (RegionData->MipIndex >= CurrentFirstMip)
					{
						RHIUpdateTexture2D(
							RegionData->Texture2DResource->GetTexture2DRHI(),
							RegionData->MipIndex - CurrentFirstMip,
							RegionData->Regions[RegionIndex],
							RegionData->SrcPitch,
							RegionData->SrcData
							+ RegionData->Regions[RegionIndex].SrcY * RegionData->SrcPitch
							+ RegionData->Regions[RegionIndex].SrcX * RegionData->SrcBpp
						);
					}
				}
		if (bFreeData)
		{
			FMemory::Free(RegionData->Regions);
			FMemory::Free(RegionData->SrcData);
		}
		delete RegionData;
			});
	}
}
