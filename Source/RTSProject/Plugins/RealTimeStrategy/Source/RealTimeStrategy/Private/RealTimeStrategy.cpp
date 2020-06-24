#include "RealTimeStrategy.h"

#include "Modules/ModuleManager.h"

#if WITH_GAMEPLAY_DEBUGGER
#include "GameplayDebugger.h"

#include "RTSGameplayDebuggerCategory.h"
#endif


void FRealTimeStrategy::StartupModule()
{
#if WITH_GAMEPLAY_DEBUGGER
    IGameplayDebugger& GameplayDebuggerModule = IGameplayDebugger::Get();
    GameplayDebuggerModule.RegisterCategory("RTS",
        IGameplayDebugger::FOnGetCategory::CreateStatic(&FRTSGameplayDebuggerCategory::MakeInstance),
        EGameplayDebuggerCategoryState::Disabled);
    GameplayDebuggerModule.NotifyCategoriesChanged();
#endif
}

void FRealTimeStrategy::ShutdownModule()
{
#if WITH_GAMEPLAY_DEBUGGER
    if (IGameplayDebugger::IsAvailable())
    {
        IGameplayDebugger& GameplayDebuggerModule = IGameplayDebugger::Get();
        GameplayDebuggerModule.UnregisterCategory("RTS");
        GameplayDebuggerModule.NotifyCategoriesChanged();
    }
#endif
}

IMPLEMENT_MODULE(FRealTimeStrategy, RealTimeStrategy)
