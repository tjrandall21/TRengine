#include <TRengine/inc/TRengine.h>
#include "ShapeStates.h"

using namespace TRengine;
using namespace TRengine::Core;
using namespace TRengine::Input;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
    AppConfig config;
    config.appName = L"Hello MeshBuilder";

    App& mainApp = MainApp();
    mainApp.AddState<ShapeState>("ShapeState");

    mainApp.Run(config);
    return 0;
}