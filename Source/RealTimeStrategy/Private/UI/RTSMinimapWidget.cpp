#include "UI/RTSMinimapWidget.h"

#include "EngineUtils.h"
#include "Components/BrushComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Rendering/DrawElements.h"

#include "RTSLog.h"
#include "RTSOwnerComponent.h"
#include "RTSPlayerController.h"
#include "RTSPlayerState.h"
#include "Combat/RTSHealthComponent.h"
#include "UI/RTSMinimapVolume.h"
#include "Vision/RTSFogOfWarActor.h"
#include "Vision/RTSVisionInfo.h"
#include "Vision/RTSVisionState.h"
#include "Vision/RTSVisionVolume.h"


URTSMinimapWidget::URTSMinimapWidget(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
	// Set reasonable default values.
	DamagedUnitBlinkTimeSeconds = 3.0f;
}

void URTSMinimapWidget::NotifyOnDrawUnit(
	FPaintContext& Context,
	AActor* Actor,
	APlayerState* ActorOwner,
	const FVector2D& MinimapPosition,
	APlayerState* LocalPlayer) const
{
	ReceiveOnDrawUnit(Context, Actor, ActorOwner, MinimapPosition, LocalPlayer);
}

void URTSMinimapWidget::NativeConstruct()
{
	UUserWidget::NativeConstruct();

	// Get minimap bounds.
	for (TActorIterator<ARTSMinimapVolume> It(GetWorld()); It; ++It)
	{
		MinimapVolume = *It;
		break;
	}

	if (MinimapVolume)
	{
		// Set size.
		const UBrushComponent* MinimapBrushComponent = MinimapVolume->GetBrushComponent();
		const FBoxSphereBounds MinimapBounds = MinimapBrushComponent->CalcBounds(MinimapBrushComponent->GetComponentTransform());

		MinimapWorldSize = MinimapBounds.BoxExtent * 2;

		// Set background image.
		MinimapBackground.SetResourceObject(MinimapVolume->GetMinimapImage());
	}
	else
	{
		UE_LOG(LogRTS, Warning, TEXT("No RTSMinimapVolume found. Minimap won't be showing unit positions."));
	}

	// Get fog of war actor.
	if (bDrawVision)
	{
		for (TActorIterator<ARTSFogOfWarActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			FogOfWarActor = *ActorItr;
			break;
		}

		if (FogOfWarActor)
		{
			// Setup fog of war material.
			FogOfWarMaterialInstance = UMaterialInstanceDynamic::Create(FogOfWarMaterial, nullptr);
		}
		else
		{
			UE_LOG(LogRTS, Warning, TEXT("No fog of war actor found, won't draw vision on minimap."));
			bDrawVision = false;
		}
	}
}

void URTSMinimapWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (!IsVisible())
	{
		return;
	}

	if (!bDrawVision)
	{
		return;
	}

	if (!IsValid(FogOfWarActor) || !IsValid(FogOfWarActor->GetFogOfWarTexture()) || !IsValid(FogOfWarMaterialInstance))
	{
		return;
	}

	// Update material.
	FogOfWarMaterialInstance->SetTextureParameterValue(FName(TEXT("VisibilityMask")),
	                                                   FogOfWarActor->GetFogOfWarTexture());

	FogOfWarBrush.ImageSize =
		FVector2D(FogOfWarActor->GetFogOfWarTexture()->GetSizeX(), FogOfWarActor->GetFogOfWarTexture()->GetSizeY());
	FogOfWarBrush.SetResourceObject(FogOfWarMaterialInstance);
}

#if ENGINE_MAJOR_VERSION > 4 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 20)
int32 URTSMinimapWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	LayerId = UUserWidget::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

	FPaintContext Context(AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

	Context.MaxLayer++;

	DrawBackground(Context);
	DrawUnits(Context);
	DrawVision(Context);
	DrawViewFrustum(Context);

	return FMath::Max(LayerId, Context.MaxLayer);
}
#else
void URTSMinimapWidget::NativePaint(FPaintContext& InContext) const
{
	UUserWidget::NativePaint(InContext);

	InContext.MaxLayer++;

	DrawBackground(InContext);
	DrawUnits(InContext);
	DrawVision(InContext);
	DrawViewFrustum(InContext);
}
#endif

FReply URTSMinimapWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// Apply default handling.
	auto Reply = UUserWidget::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (Reply.IsEventHandled())
	{
		return Reply;
	}

	// Handle MouseMove events from now on.
	bMouseDown = true;

	// Handle initial click.
	return HandleMinimapClick(InGeometry, InMouseEvent);
}

FReply URTSMinimapWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// Apply default handling.
	auto Reply = UUserWidget::NativeOnMouseButtonUp(InGeometry, InMouseEvent);

	if (Reply.IsEventHandled())
	{
		return Reply;
	}

	// Stop handling MouseMove events.
	bMouseDown = false;
	return FReply::Handled();
}

FReply URTSMinimapWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// Apply default handling.
	auto Reply = UUserWidget::NativeOnMouseMove(InGeometry, InMouseEvent);

	if (Reply.IsEventHandled())
	{
		return Reply;
	}

	if (bMouseDown)
	{
		// Handle movement.
		return HandleMinimapClick(InGeometry, InMouseEvent);
	}

	return FReply::Unhandled();
}

void URTSMinimapWidget::DrawBackground(FPaintContext& InContext) const
{
	if (!bDrawBackground)
	{
		return;
	}

	DrawBoxWithBrush(InContext, FVector2D(0, 0), MinimapBackground);
}

void URTSMinimapWidget::DrawUnits(FPaintContext& InContext) const
{
	if (!MinimapVolume)
	{
		return;
	}

	APlayerController* Player = GetOwningPlayer();

	ARTSPlayerState* PlayerState = Player->GetPlayerState<ARTSPlayerState>();

	for (TActorIterator<AActor> ActorIt(GetWorld()); ActorIt; ++ActorIt)
	{
		AActor* Actor = *ActorIt;

		if (!IsValid(Actor))
		{
			continue;
		}

		if (Actor->IsHidden())
		{
			continue;
		}

		const URTSOwnerComponent* OwnerComponent = Actor->FindComponentByClass<URTSOwnerComponent>();

		FVector ActorLocationWorld = Actor->GetActorLocation();
		FVector2D ActorLocationMinimap = WorldToMinimap(ActorLocationWorld);

		// Draw on minimap.
		if (bDrawUnitsWithTeamColors && OwnerComponent)
		{
			if (OwnerComponent->GetPlayerOwner() == PlayerState)
			{
				// Check if the unit has been taking damage lately.
				URTSHealthComponent* HealthComponent = Actor->FindComponentByClass<URTSHealthComponent>();
				float RealTimeSeconds = Actor->GetWorld()->GetRealTimeSeconds();

				FSlateBrush UnitSlateBrush = OwnUnitsBrush;
				if (bUsePlayerColors)
				{
					UnitSlateBrush.TintColor = PlayerState->GetColor();
				}

				if (!IsValid(HealthComponent) ||
					HealthComponent->GetLastTimeDamageTaken() <= 0.0f ||
					HealthComponent->GetLastTimeDamageTaken() + DamagedUnitBlinkTimeSeconds < RealTimeSeconds)
				{
					DrawBoxWithBrush(InContext, ActorLocationMinimap, UnitSlateBrush);
				}
				else
				{
					// Have unit blink while taking damage.
					if (RealTimeSeconds - FMath::FloorToFloat(RealTimeSeconds) <= 0.5)
					{
						DrawBoxWithBrush(InContext, ActorLocationMinimap, UnitSlateBrush);
					}
					else
					{
						DrawBoxWithBrush(InContext, ActorLocationMinimap, DamagedUnitsBlinkBrush);
					}
				}
			}
			else if (OwnerComponent->GetPlayerOwner() != nullptr && !OwnerComponent->IsSameTeamAsController(Player))
			{
				
				FSlateBrush UnitSlateBrush = EnemyUnitsBrush;
				if (bUsePlayerColors)
				{
					UnitSlateBrush.TintColor = OwnerComponent->GetPlayerOwner()->GetColor();
				}
				
				DrawBoxWithBrush(InContext, ActorLocationMinimap, UnitSlateBrush);
			}
			else
			{
				DrawBoxWithBrush(InContext, ActorLocationMinimap, NeutralUnitsBrush);
			}
		}

		// Allow custom drawing.
		NotifyOnDrawUnit(
			InContext,
			Actor,
			OwnerComponent ? OwnerComponent->GetPlayerOwner() : nullptr,
			ActorLocationMinimap,
			Player->PlayerState);
	}
}

void URTSMinimapWidget::DrawVision(FPaintContext& InContext) const
{
	if (!bDrawVision)
	{
		return;
	}

	if (!FogOfWarActor || !FogOfWarActor->GetFogOfWarTexture() || !FogOfWarMaterialInstance)
	{
		return;
	}

	DrawBoxWithBrush(InContext, FVector2D::ZeroVector, FogOfWarBrush);
}

void URTSMinimapWidget::DrawViewFrustum(FPaintContext& InContext) const
{
	if (!bDrawViewFrustum)
	{
		return;
	}

	// Get viewport size.
	ARTSPlayerController* Player = Cast<ARTSPlayerController>(GetOwningPlayer());

	if (!Player)
	{
		return;
	}

	int32 ViewportWidth;
	int32 ViewportHeight;

	Player->GetViewportSize(ViewportWidth, ViewportHeight);

	// Cast four rays.
	const FVector2D ViewportTopLeft(0, 0);
	const FVector2D ViewportTopRight(ViewportWidth, 0);
	const FVector2D ViewportBottomLeft(0, ViewportHeight);
	const FVector2D ViewportBottomRight(ViewportWidth, ViewportHeight);

	FVector WorldTopLeft;
	FVector WorldTopRight;
	FVector WorldBottomLeft;
	FVector WorldBottomRight;

	ViewportToWorld(Player, ViewportTopLeft, WorldTopLeft);
	ViewportToWorld(Player, ViewportTopRight, WorldTopRight);
	ViewportToWorld(Player, ViewportBottomLeft, WorldBottomLeft);
	ViewportToWorld(Player, ViewportBottomRight, WorldBottomRight);

	// Convert to minimap space.
	const FVector2D MinimapTopLeft = WorldToMinimap(WorldTopLeft);
	const FVector2D MinimapTopRight = WorldToMinimap(WorldTopRight);
	const FVector2D MinimapBottomLeft = WorldToMinimap(WorldBottomLeft);
	const FVector2D MinimapBottomRight = WorldToMinimap(WorldBottomRight);

	// Draw view frustum.
	TArray<FVector2D> Points;

	Points.Add(MinimapTopLeft);
	Points.Add(MinimapTopRight);
	Points.Add(MinimapBottomRight);
	Points.Add(MinimapBottomLeft);
	Points.Add(MinimapTopLeft);

	FSlateDrawElement::MakeLines(
		InContext.OutDrawElements,
		InContext.MaxLayer,
		InContext.AllottedGeometry.ToPaintGeometry(),
		Points);
}

void URTSMinimapWidget::DrawBoxWithBrush(FPaintContext& InContext, const FVector2D& Position, const FSlateBrush& Brush) const
{
	FSlateDrawElement::MakeBox(
		InContext.OutDrawElements,
		InContext.MaxLayer,
		InContext.AllottedGeometry.ToPaintGeometry(Position, Brush.ImageSize),
		&Brush,
		ESlateDrawEffect::None,
		Brush.TintColor.GetSpecifiedColor());
}

FReply URTSMinimapWidget::HandleMinimapClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	ARTSPlayerController* Player = Cast<ARTSPlayerController>(GetOwningPlayer());

	if (!Player)
	{
		return FReply::Unhandled();
	}

	// Convert clicked minimap position to world space.
	const FVector2D MinimapPosition = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
	const FVector WorldPosition = MinimapToWorld(MinimapPosition);

	// Notify player.
	Player->NotifyOnMinimapClicked(InMouseEvent, MinimapPosition, WorldPosition);
	return FReply::Handled();
}

FVector URTSMinimapWidget::MinimapToWorld(const FVector2D& MinimapPosition) const
{
	// Convert to relative minimap position.
	float RelativeMinimapX = MinimapPosition.X / MinimapBackground.ImageSize.X;
	float RelativeMinimapY = MinimapPosition.Y / MinimapBackground.ImageSize.Y;

	// Rotate to match UI coordinate system.
	const float Temp = RelativeMinimapX;
	RelativeMinimapX = 1 - RelativeMinimapY;
	RelativeMinimapY = Temp;

	// Convert to world coordinates.
	const float WorldX = (RelativeMinimapX - 0.5) * MinimapWorldSize.X;
	const float WorldY = (RelativeMinimapY - 0.5) * MinimapWorldSize.Y;

	return FVector(WorldX, WorldY, 0.0f);
}

bool URTSMinimapWidget::ViewportToWorld(ARTSPlayerController* Player, const FVector2D& ViewportPosition, FVector& OutWorldPosition) const
{
	// Get ray.
	FVector WorldOrigin;
	FVector WorldDirection;
	if (!UGameplayStatics::DeprojectScreenToWorld(Player, ViewportPosition, WorldOrigin, WorldDirection))
	{
		return false;
	}

	// Make plane.
	const FPlane ZPlane = FPlane(FVector::ZeroVector, FVector::UpVector);

	// Calculate intersection point.
	OutWorldPosition = FMath::LinePlaneIntersection(WorldOrigin, WorldOrigin + WorldDirection * 1000.0f, ZPlane);
	return true;
}

FVector2D URTSMinimapWidget::WorldToMinimap(const FVector& WorldPosition) const
{
	// Get relative world position.
	float RelativeWorldX = WorldPosition.X / MinimapWorldSize.X + 0.5f;
	float RelativeWorldY = WorldPosition.Y / MinimapWorldSize.Y + 0.5f;

	// Rotate to match UI coordinate system.
	const float Temp = RelativeWorldX;
	RelativeWorldX = RelativeWorldY;
	RelativeWorldY = 1 - Temp;

	// Convert to minimap coordinates.
	const float MinimapX = RelativeWorldX * MinimapBackground.ImageSize.X;
	const float MinimapY = RelativeWorldY * MinimapBackground.ImageSize.Y;

	return FVector2D(MinimapX, MinimapY);
}
