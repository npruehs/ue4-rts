#include "UI/RTSMinimapVolume.h"

#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Engine/Texture2D.h"
#include "Kismet/GameplayStatics.h"


ARTSMinimapVolume::ARTSMinimapVolume(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	// Create capture component for generating top-down snapshot
	CaptureComponent2D = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("CaptureComponent2D"));
	CaptureComponent2D->SetupAttachment(GetRootComponent());

	// Loads default render target asset to store generated snapshot in
	static ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> RenderTargetAssetFinder(TEXT("/RealTimeStrategy/Minimap/RT_MinimapSnapshot"));
	CaptureComponent2D->TextureTarget = RenderTargetAssetFinder.Object;

	// Point camera downward
	CaptureComponent2D->SetWorldRotation(FRotator(-90, 0, 0));
	CaptureComponent2D->ProjectionType = ECameraProjectionMode::Type::Orthographic;
	CaptureComponent2D->CaptureSource = SCS_FinalColorLDR;
	CaptureComponent2D->bCaptureEveryFrame = false;

	HiddenActorClasses.Add(APawn::StaticClass());
}

#if WITH_EDITOR
void ARTSMinimapVolume::PostEditMove(const bool bFinished)
{
	Super::PostEditMove(bFinished);
	GenerateSnapshot();
}
#endif

UTexture* ARTSMinimapVolume::GetMinimapImage() const
{
	return IsValid(MinimapImage) ? MinimapImage : static_cast<UTexture*>(CaptureComponent2D->TextureTarget.Get());
}

void ARTSMinimapVolume::BeginPlay()
{
	if (MinimapImage == nullptr)
	{
		GenerateSnapshot();
	}

	Super::BeginPlay();
}

void ARTSMinimapVolume::GenerateSnapshot()
{
	UE_LOG(LogTemp, Warning, TEXT("ARTSMinimapVolume::GenerateSnapshot"))
	// Undo any scale inherited from root component
	CaptureComponent2D->SetWorldScale3D(FVector(1.0f));

	// Set the orthographic width to whichever of the XY extent is largest
	const FVector ScaledBoxExtent = GetBounds().BoxExtent;
	const float SnapshotRadius = FMath::Max(ScaledBoxExtent.X, ScaledBoxExtent.Y);
	CaptureComponent2D->OrthoWidth = 2.0f * SnapshotRadius;

	// Move sufficiently up so all level geometry is captured
	CaptureComponent2D->SetRelativeLocation(FVector(0, 0, 2 * GetBounds().BoxExtent.Z));

	CaptureComponent2D->HiddenActors.Empty();
	CaptureComponent2D->HiddenComponents.Empty();


	HiddenActorClasses.Remove(nullptr);
	for (const TSubclassOf<AActor> HiddenActorClass : HiddenActorClasses)
	{
		TArray<AActor*> ActorsOfClass;
		UGameplayStatics::GetAllActorsOfClass(this, HiddenActorClass, ActorsOfClass);
		for (AActor* FoundActor : ActorsOfClass)
		{
			CaptureComponent2D->HideActorComponents(FoundActor);
		}
	}

	HiddenActors.Remove(nullptr);
	for (AActor* HiddenActor : HiddenActors)
	{
		CaptureComponent2D->HideActorComponents(HiddenActor);
	}

	CaptureComponent2D->CaptureScene();
}
