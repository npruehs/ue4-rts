#pragma once

#include "CoreMinimal.h"

#include "UObject/Object.h"

#include "Orders/RTSOrderGroupExecutionType.h"
#include "Orders/RTSOrderTagRequirements.h"
#include "Orders/RTSOrderTargetData.h"
#include "Orders/RTSOrderTargetType.h"

#include "RTSOrder.generated.h"


class UTexture2D;


/** Order that can be issued to an actor. */
UCLASS(BlueprintType, Blueprintable)
class REALTIMESTRATEGY_API URTSOrder : public UObject
{
    GENERATED_BODY()

public:
    /** Whether the specified actor and/or location is a valid target for this order. */
    virtual bool IsValidTarget(const AActor* OrderedActor, const FRTSOrderTargetData& TargetData, int32 Index) const;

    /** Gets the target required by this order. */
    ERTSOrderTargetType GetTargetType() const;

    /** Gets how many and which of the selected actors the order should be issued to. */
    ERTSOrderGroupExecutionType GetGroupExecutionType() const;

    /** Gets the tag requirements that must be fulfilled to issue the order. */
    FRTSOrderTagRequirements GetIssueTagRequirements() const;

    /** Gets the normal icon of the order. */
    UTexture2D* GetNormalIcon() const;

    /** Gets the hovered icon of the order. */
    UTexture2D* GetHoveredIcon() const;

    /** Gets the pressed icon of the order. */
    UTexture2D* GetPressedIcon() const;

    /** Gets the disabled icon of the order. */
    UTexture2D* GetDisabledIcon() const;

    /** Gets the name of the order. */
    FText GetName() const;

    /** Gets the description of the order. */
    virtual FText GetDescription(const AActor* OrderedActor, int32 Index) const;

protected:
    /** Target required by this order. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS")
    ERTSOrderTargetType TargetType;

    /** How many and which of the selected actors the order should be issued to. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS")
    ERTSOrderGroupExecutionType GroupExecutionType;

    /** Tag requirements that must be fulfilled to issue the order. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS")
    FRTSOrderTagRequirements IssueTagRequirements;

    /** Tag requirements that must be fulfilled for the order to be successful. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS")
    FRTSOrderTagRequirements SuccessTagRequirements;

    /** Normal icon of the order. Can be shown in the UI. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS")
    UTexture2D* NormalIcon;

    /** Hovered icon of the order. Can be shown in the UI. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS")
    UTexture2D* HoveredIcon;

    /** Pressed icon of the order. Can be shown in the UI. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS")
    UTexture2D* PressedIcon;

    /** Disabled icon of the order. Can be shown in the UI. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS")
    UTexture2D* DisabledIcon;

    /** Name of the order. Can be shown in the UI. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS")
    FText Name;

    /** Default description of the order. Can be shown in the UI. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS")
    FText Description;
};
