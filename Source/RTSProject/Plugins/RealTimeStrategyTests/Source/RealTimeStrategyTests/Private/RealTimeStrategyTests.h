#pragma once

#include "IRealTimeStrategyTests.h"

class FRealTimeStrategyTests : public IRealTimeStrategyTests
{
    /** IModuleInterface implementation */
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
