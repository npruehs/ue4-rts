#pragma once

#include "CoreMinimal.h"

#include "RTSResourceType.generated.h"


class UTexture2D;


/** Resource that can be gathered and spent for construction, production and research. */
UCLASS(Category="RTS", Blueprintable, BlueprintType)
class REALTIMESTRATEGY_API URTSResourceType : public UObject
{
	GENERATED_BODY()

public:
	/** Gets the display name of the resource. */
	UFUNCTION(BlueprintPure)
	FText GetResourceName() const;

	/** Gets the icon to show next to resource amounts in the UI. */
	UFUNCTION(BlueprintPure)
	UTexture2D* GetResourceIcon() const;

	/** Gets the color to use when showing resource amounts in the UI. */
	UFUNCTION(BlueprintPure)
	FLinearColor GetResourceColor() const;

private:
	/** Display name of the resource. */
	UPROPERTY(EditDefaultsOnly, Category = "RTS")
	FText ResourceName;

	/** Icon to show next to resource amounts in the UI. */
	UPROPERTY(EditDefaultsOnly, Category = "RTS")
	UTexture2D* ResourceIcon;

	/** Color to use when showing resource amounts in the UI. */
	UPROPERTY(EditDefaultsOnly, Category = "RTS")
	FLinearColor ResourceColor;
};
