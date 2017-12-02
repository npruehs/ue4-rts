#include "RTSPluginPrivatePCH.h"

#include "Modules/ModuleManager.h"

#include "RTSPlugin.h"


void FRTSPlugin::StartupModule()
{
	// This code will execute after your module is loaded into memory (but after global variables are initialized, of course.)
}

void FRTSPlugin::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

IMPLEMENT_MODULE(FRTSPlugin, RTSPlugin)

DEFINE_LOG_CATEGORY(LogRTS);
