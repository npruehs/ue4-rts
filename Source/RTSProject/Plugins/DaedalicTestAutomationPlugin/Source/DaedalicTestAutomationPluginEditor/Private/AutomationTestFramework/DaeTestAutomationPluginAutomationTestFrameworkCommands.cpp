#include "AutomationTestFramework/DaeTestAutomationPluginAutomationTestFrameworkCommands.h"
#include "DaeTestSuiteActor.h"
#include <Editor.h>
#include <EngineUtils.h>
#include <Editor/UnrealEdEngine.h>

bool FDaeTestAutomationPluginWaitForEndOfTestSuite::Update()
{
    if (!GEditor)
    {
        return false;
    }

    if (!GEditor->PlayWorld)
    {
        return false;
    }

    if (!Context.CurrentTestSuite)
    {
        for (TActorIterator<ADaeTestSuiteActor> Iter(GEditor->PlayWorld); Iter; ++Iter)
        {
            Context.CurrentTestSuite = *Iter;
        }
    }

    if (Context.CurrentTestSuite)
    {
        return !Context.CurrentTestSuite->IsRunning();
    }

    return false;
}
