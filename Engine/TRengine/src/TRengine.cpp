#include "Precompiled.h"
#include "TRengine.h"

TRengine::App& TRengine::MainApp()
{
    static App sApp;
    return sApp;
}