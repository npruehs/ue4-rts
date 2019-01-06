#include "RealTimeStrategyPCH.h"

#include "Modules/ModuleManager.h"

#include "RealTimeStrategy.h"


void FRealTimeStrategy::StartupModule()
{
	// This code will execute after your module is loaded into memory (but after global variables are initialized, of course.)
}

void FRealTimeStrategy::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

IMPLEMENT_MODULE(FRealTimeStrategy, RealTimeStrategy)

DEFINE_LOG_CATEGORY(LogRTS);
