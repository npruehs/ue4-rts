#pragma once

#include "IRTSPlugin.h"

class FRTSPlugin : public IRTSPlugin
{
    /** IModuleInterface implementation */
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
