#pragma once

#include <GauntletModule.h>

struct DAEDALICTESTAUTOMATIONPLUGIN_API FDaeGauntletStates : FGauntletStates
{
    static FName LoadingNextMap;
    static FName DiscoveringTests;
    static FName Running;
    static FName Finished;
};
