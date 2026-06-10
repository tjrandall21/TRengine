#include <TRengine/inc/TRengine.h>

using namespace TRengine;
using namespace TRengine::Core;
using namespace TRengine::Input;

class MainState : public AppState
{
public:
    void Initialize() override
    {
        LOG("MainState: Initialize");
        mStartTime = TimeUtil::GetTime();
    }
    void Update(float deltaTime) override
    {
        if (InputSystem::Get()->IsKeyPressed(KeyCode::G))
        {
            MainApp().ChangeState("GameState");
        }
    }
    void Terminate() override
    {
        LOG("MainState: Terminate");
    }
private:
    float mStartTime = 0.0f;
};

class GameState : public AppState
{
public:
    void Initialize() override
    {
        LOG("GameState: Initialize");
        mStartTime = TimeUtil::GetTime();
    }
    void Update(float deltaTime) override
    {
        if (InputSystem::Get()->IsKeyPressed(KeyCode::M))
        {
            MainApp().ChangeState("MainState");
        }
    }
    void Terminate() override
    {
        LOG("GameState: Terminate");
    }
private:
    float mStartTime = 0.0f;
};

int WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
    AppConfig config;
    config.appName = L"Hello Window";

    App& mainApp = MainApp();
    mainApp.AddState<MainState>("MainState");
    mainApp.AddState<GameState>("GameState");
    mainApp.Run(config);
    return 0;
}
