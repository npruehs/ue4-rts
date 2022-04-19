#include "Vision/RTSVisionTile.h"

#include "GameFramework/Actor.h"


void FRTSVisionTile::AddActor(AActor* Actor)
{
	++NumActors;
}

void FRTSVisionTile::RemoveActor(AActor* Actor)
{
	--NumActors;
}

bool FRTSVisionTile::IsVisible() const
{
	return NumActors > 0;
}
