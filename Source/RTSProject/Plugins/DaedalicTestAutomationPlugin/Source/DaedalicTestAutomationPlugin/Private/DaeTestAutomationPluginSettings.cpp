#include "DaeTestAutomationPluginSettings.h"

UDaeTestAutomationPluginSettings::UDaeTestAutomationPluginSettings()
    : TestMapPath(TEXT("Maps/AutomatedTests"))
{
}

#if WITH_EDITOR
void UDaeTestAutomationPluginSettings::PostEditChangeProperty(
    struct FPropertyChangedEvent& PropertyChangedEvent)
{
    if (PropertyChangedEvent.GetPropertyName() == TEXT("TestMapPath"))
    {
        OnTestMapPathChanged.Broadcast(TestMapPath);
    }
}
#endif
