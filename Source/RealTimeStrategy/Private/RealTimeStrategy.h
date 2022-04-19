#pragma once

#include "IRealTimeStrategy.h"

class FRealTimeStrategy : public IRealTimeStrategy
{
    /** IModuleInterface implementation */
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
