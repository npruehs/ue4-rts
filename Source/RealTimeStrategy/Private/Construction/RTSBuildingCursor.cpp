#include "Construction/RTSBuildingCursor.h"

#include "NavigationSystem.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Texture2D.h"
#include "Engine/World.h"
#include "NavFilters/RecastFilter_UseDefaultArea.h"

#include "Combat/RTSAttackComponent.h"
#include "Combat/RTSCombatComponent.h"
#include "Construction/RTSConstructionSiteComponent.h"
#include "Libraries/RTSCollisionLibrary.h"
#include "Libraries/RTSGameplayLibrary.h"


ARTSBuildingCursor::ARTSBuildingCursor(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	// Set reasonable default values.
	GridCellSize = 100.0f;

	bCheckCollision = true;
	BuildingLocationDetectionChannels.Add(EObjectTypeQuery::ObjectTypeQuery2); // WorldDynamic
	BuildingLocationDetectionChannels.Add(EObjectTypeQuery::ObjectTypeQuery3); // Pawn

	bCheckNavigation = false;
	NavigationQueryFilterClass = URecastFilter_UseDefaultArea::StaticClass();
	NavigationQueryExtent = FVector(10.0f, 10.0f, 1000.0f);

	GridWidthAndHeight = 0;
	GridTextureBuffer = nullptr;
}

void ARTSBuildingCursor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (IsValid(RangeIndicator))
	{
		RangeIndicator->Destroy();
	}
}

void ARTSBuildingCursor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!HasGrid())
	{
		return;
	}

	// Check all cells.
	bAllCellsValid = true;

	for (int32 Y = 0; Y < GridWidthAndHeight; ++Y)
	{
		for (int32 X = 0; X < GridWidthAndHeight; ++X)
		{
			const int i = Y * GridWidthAndHeight + X;

			const int iBlue = i * 4 + 0;
			const int iGreen = i * 4 + 1;
			const int iRed = i * 4 + 2;
			const int iAlpha = i * 4 + 3;

			const float HalfGridOffset = (GridWidthAndHeight % 2 == 0) ? GridCellSize / 2.0f : 0.0f;

			FVector WorldLocation = GetActorLocation();
			WorldLocation.X += GridCellSize * X - (GridCellSize * GridWidthAndHeight / 2) + HalfGridOffset;
			WorldLocation.Y += GridCellSize * Y - (GridCellSize * GridWidthAndHeight / 2) + HalfGridOffset;

			if (CanConstructBuildingAt(WorldLocation))
			{
				GridTextureBuffer[iBlue] = 0;
				GridTextureBuffer[iGreen] = 0;
				GridTextureBuffer[iRed] = 0;
				GridTextureBuffer[iAlpha] = 0;
			}
			else
			{
				GridTextureBuffer[iBlue] = 0;
				GridTextureBuffer[iGreen] = 0;
				GridTextureBuffer[iRed] = 255;
				GridTextureBuffer[iAlpha] = 0;

				bAllCellsValid = false;
			}
		}
	}

	GridTexture->UpdateTextureRegions(0, 1, &GridUpdateTextureRegion, GridWidthAndHeight * 4, (uint32)4, GridTextureBuffer);
}

void ARTSBuildingCursor::SetupForBuilding(TSubclassOf<AActor> BuildingClass)
{
	// Set mesh.
	const UStaticMeshComponent* StaticMeshComponent = URTSGameplayLibrary::FindDefaultComponentByClass<UStaticMeshComponent>(BuildingClass);

	if (IsValid(StaticMeshComponent))
	{
		SetStaticMesh(StaticMeshComponent->GetStaticMesh(), StaticMeshComponent->GetRelativeTransform());
	}
	else
	{
		const USkeletalMeshComponent* SkeletalMeshComponent = URTSGameplayLibrary::FindDefaultComponentByClass<USkeletalMeshComponent>(BuildingClass);

		if (IsValid(SkeletalMeshComponent))
		{
			SetSkeletalMesh(SkeletalMeshComponent->SkeletalMesh, SkeletalMeshComponent->GetRelativeTransform());
		}
	}

	// Update valid indicator.
	SetLocationValid(false);

	// Check if we should preview attack range.
	const URTSConstructionSiteComponent* ConstructionSiteComponent =
		URTSGameplayLibrary::FindDefaultComponentByClass<URTSConstructionSiteComponent>(BuildingClass);

	bool bPreviewAttackRange = false;
	float AttackRange = 0.0f;

	if (IsValid(ConstructionSiteComponent))
	{
		// Set grid size.
		SetGridWidthAndHeight(ConstructionSiteComponent->GetGridWidthAndHeight());

		// Preview attack range.
		bPreviewAttackRange = ConstructionSiteComponent->ShouldPreviewAttackRange();

		if (bPreviewAttackRange)
		{
			const URTSAttackComponent* AttackComponent =
				URTSGameplayLibrary::FindDefaultComponentByClass<URTSAttackComponent>(BuildingClass);

			if (IsValid(AttackComponent))
			{
				for (const FRTSAttackData& Attack : AttackComponent->GetAttacks())
				{
					if (Attack.Range > AttackRange)
					{
						AttackRange = Attack.Range;
					}
				}

				AttackRange += URTSCollisionLibrary::GetCollisionSize(BuildingClass) / 2.0f;
			}
			else
			{
				const URTSCombatComponent* CombatComponent = URTSGameplayLibrary::FindDefaultComponentByClass<URTSCombatComponent>(BuildingClass); 
				if (IsValid(CombatComponent))
				{
					for (const FRTSAttackData& Attack : CombatComponent->GetAttacks())
					{
						if (Attack.Range > AttackRange)
						{
							AttackRange = Attack.Range;
						}
					}

					AttackRange += URTSCollisionLibrary::GetCollisionSize(BuildingClass) / 2.0f;
				}
				else
				{
					bPreviewAttackRange = false;
				}
			}
		}
	}

	if (bPreviewAttackRange && AttackRange > 0.0f)
	{
		SetRange(AttackRange);
	}
}

UTexture2D* ARTSBuildingCursor::GetGridTexture() const
{
	return GridTexture;
}

int32 ARTSBuildingCursor::GetGridWidthAndHeight() const
{
	return GridWidthAndHeight;
}

void ARTSBuildingCursor::SetGridWidthAndHeight(int32 InGridWidthAndHeight)
{
	GridWidthAndHeight = InGridWidthAndHeight;

	// Build grid texture.
	if (GridTextureBuffer)
	{
		delete GridTextureBuffer;
	}

	if (GridWidthAndHeight <= 0)
	{
		return;
	}

	GridTextureBuffer = new uint8[GridWidthAndHeight * GridWidthAndHeight * 4];

	GridTexture = UTexture2D::CreateTransient(GridWidthAndHeight, GridWidthAndHeight);

#if WITH_EDITORONLY_DATA
	GridTexture->MipGenSettings = TextureMipGenSettings::TMGS_NoMipmaps;
#endif

	GridTexture->Filter = TF_Nearest;
	GridTexture->AddToRoot();

	GridTexture->UpdateResource();

	GridUpdateTextureRegion = FUpdateTextureRegion2D(0, 0, 0, 0, GridWidthAndHeight, GridWidthAndHeight);
}

float ARTSBuildingCursor::GetGridCellSize() const
{
	return GridCellSize;
}

bool ARTSBuildingCursor::HasGrid() const
{
	return GridWidthAndHeight > 0;
}

bool ARTSBuildingCursor::AreAllCellsValid() const
{
	return bAllCellsValid;
}

void ARTSBuildingCursor::SetCursorLocation(const FVector& NewWorldLocation)
{
	FVector FinalLocation = NewWorldLocation;

	if (HasGrid())
	{
		// Snap to grid.
		const float X = FMath::GridSnap(NewWorldLocation.X, GridCellSize);
		const float Y = FMath::GridSnap(NewWorldLocation.Y, GridCellSize);

		FinalLocation = FVector(X, Y, NewWorldLocation.Z);
	}

	SetActorLocation(FinalLocation);
}

void ARTSBuildingCursor::SetRange(float Range)
{
	if (!RangeIndicatorClass)
	{
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	RangeIndicator = GetWorld()->SpawnActor<ARTSRangeIndicator>(RangeIndicatorClass, SpawnParams);
	RangeIndicator->SetRange(Range);
	RangeIndicator->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
}

bool ARTSBuildingCursor::CanConstructBuildingAt(const FVector& WorldLocation)
{
	if (bCheckCollision)
	{
		// Check for colliders.
		const FCollisionObjectQueryParams CollisionObjectQueryParams(BuildingLocationDetectionChannels);
		const FCollisionQueryParams CollisionQueryParams;

		if (GetWorld()->LineTraceTestByObjectType(WorldLocation + FVector(0.0f, 0.0f, 10000.0f),
		                                          WorldLocation - FVector(0.0f, 0.0f, 10000.0f),
		                                          CollisionObjectQueryParams, CollisionQueryParams))
		{
			return false;
		}
	}

	if (bCheckNavigation)
	{
		FVector ProjectedLocation;

		if (!UNavigationSystemV1::K2_ProjectPointToNavigation(this, WorldLocation, ProjectedLocation, nullptr,
		                                                      NavigationQueryFilterClass, NavigationQueryExtent))
		{
			return false;
		}
	}

	return true;
}
