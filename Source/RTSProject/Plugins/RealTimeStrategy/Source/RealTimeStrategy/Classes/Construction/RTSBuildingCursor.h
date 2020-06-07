#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "UI/RTSRangeIndicator.h"

#include "RTSBuildingCursor.generated.h"


class USkeletalMesh;
class UStaticMesh;


/** 3D cursor for selecting a location for a building. */
UCLASS()
class REALTIMESTRATEGY_API ARTSBuildingCursor : public AActor
{
	GENERATED_BODY()

public:
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


    /** Sets the preview skeletal mesh of this cursor. */
    UFUNCTION(BlueprintImplementableEvent, Category = "RTS")
    void SetSkeletalMesh(USkeletalMesh* BuildingMesh, const FTransform& RelativeTransform);

    /** Sets the preview static mesh of this cursor. */
    UFUNCTION(BlueprintImplementableEvent, Category = "RTS")
    void SetStaticMesh(UStaticMesh* BuildingMesh, const FTransform& RelativeTransform);

	/** Applies the visuals for either a valid or an invalid location. */
    UFUNCTION(BlueprintImplementableEvent, Category = "RTS")
	void SetLocationValid(bool bValid);

    /** Sets the range to preview while this cursor is shown. */
    void SetRange(float Range);

private:
    /** Preview to use for attack ranges. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS")
    TSubclassOf<ARTSRangeIndicator> RangeIndicatorClass;

    UPROPERTY()
    ARTSRangeIndicator* RangeIndicator;
};
