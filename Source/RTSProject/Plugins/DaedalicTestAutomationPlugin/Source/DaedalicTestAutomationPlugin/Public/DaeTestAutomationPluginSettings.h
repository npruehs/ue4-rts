#pragma once

#include <CoreMinimal.h>
#include <UObject/Object.h>
#include "DaeTestAutomationPluginSettings.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FDaeTestAutomationPluginSettingsTestMapPathChangedSignature,
                                    const FString&);

/** Custom settings for this plugin. */
UCLASS(config = Game, defaultconfig)
class DAEDALICTESTAUTOMATIONPLUGIN_API UDaeTestAutomationPluginSettings : public UObject
{
    GENERATED_BODY()

public:
    /** Path to look for test maps in. */
    UPROPERTY(config, EditAnywhere)
    FString TestMapPath;

    UDaeTestAutomationPluginSettings();

    virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

    /** Event when the path to look for test maps in has changed. */
    FDaeTestAutomationPluginSettingsTestMapPathChangedSignature OnTestMapPathChanged;
};
