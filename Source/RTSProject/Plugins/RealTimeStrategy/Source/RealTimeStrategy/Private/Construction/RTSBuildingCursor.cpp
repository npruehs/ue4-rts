#include "Construction/RTSBuildingCursor.h"

#include "Engine/World.h"


void ARTSBuildingCursor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    if (IsValid(RangeIndicator))
    {
        RangeIndicator->Destroy();
    }
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
