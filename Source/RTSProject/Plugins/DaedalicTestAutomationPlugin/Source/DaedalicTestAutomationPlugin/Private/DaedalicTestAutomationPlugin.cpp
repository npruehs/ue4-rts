// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "IDaedalicTestAutomationPlugin.h"


class FDaedalicTestAutomationPlugin : public IDaedalicTestAutomationPlugin
{
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

IMPLEMENT_MODULE( FDaedalicTestAutomationPlugin, DaedalicTestAutomationPlugin )



void FDaedalicTestAutomationPlugin::StartupModule()
{
	// This code will execute after your module is loaded into memory (but after global variables are initialized, of course.)
}


void FDaedalicTestAutomationPlugin::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}



