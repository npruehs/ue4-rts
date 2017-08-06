#include "RTSPluginPrivatePCH.h"
#include "RTSBuildingCursor.h"

#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Materials/Material.h"


ARTSBuildingCursor::ARTSBuildingCursor()
{
	Mesh = CreateOptionalDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	SetRootComponent(Mesh);
}

void ARTSBuildingCursor::SetMesh(USkeletalMesh* BuildingMesh, const FVector& MeshRelativeScale3D)
{
	Mesh->SetSkeletalMesh(BuildingMesh);
	SetActorRelativeScale3D(MeshRelativeScale3D);
}

void ARTSBuildingCursor::SetValidLocation()
{
	Mesh->SetMaterial(0, ValidLocationMaterial);
}

void ARTSBuildingCursor::SetInvalidLocation()
{
	Mesh->SetMaterial(0, InvalidLocationMaterial);
}
