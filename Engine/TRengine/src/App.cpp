#include "Precompiled.h"
#include "App.h"
#include "AppState.h"

using namespace TRengine;
using namespace TRengine::Core;
using namespace TRengine::Input;
using namespace TRengine::Graphics;

void App::Run(const AppConfig& config)
{
    LOG("App Started");

    Window myWindow;
    myWindow.Initialize(
        GetModuleHandle(nullptr),
        config.appName,
        config.winWidth,
        config.winHeight);

    auto handle = myWindow.GetWindowHandle();
    // initialize singletons
    InputSystem::StaticInitialize(handle);
    GraphicsSystem::StaticInitialize(handle, config.fullScreen);
    TextureManager::StaticInitialize(L"../../Assets/Textures");
    DebugUI::StaticInitialize(handle, false, true);
    SimpleDraw::StaticInitialize(config.maxVertexCount);

    // after initializing singletons, initialize current state
    ASSERT(mCurrentState != nullptr, "App: need an app state to run");
    mCurrentState->Initialize();

    InputSystem* input = InputSystem::Get();
    mRunning = true;
    while (mRunning)
    {
        input->Update();
        myWindow.ProcessMessage();
        if (!myWindow.IsActive() || input->IsKeyPressed(KeyCode::ESCAPE))
        {
            Quit();
            continue;
        }
        if (mNextState != nullptr)
        {
            mCurrentState->Terminate();
            mCurrentState = std::exchange(mNextState, nullptr);
            mCurrentState->Initialize();
        }
        if (input->IsKeyPressed(KeyCode::L))
        {
            GraphicsSystem::Get()->SetClearColor(Colors::White);
        }
        else if (input->IsKeyPressed(KeyCode::SEMICOLON))
        {
            GraphicsSystem::Get()->SetClearColor(Colors::Black);
        }

        float deltaTime = TimeUtil::GetDeltaTime();
#if defined(_DEBUG)
        if (deltaTime > 0.5f)
        {
            LOG("App: really long deltatime, skipping frame %f", deltaTime);
            continue;
        }
#endif
        mCurrentState->Update(deltaTime);

        // render flow
        GraphicsSystem* gs = GraphicsSystem::Get();
        gs->BeginRender();
            mCurrentState->Render();
            DebugUI::BeginDraw();
                mCurrentState->DebugUI();
            DebugUI::EndDraw();
        gs->EndRender();
    }
    //terminate active state
    mCurrentState->Terminate();

    //terminate singletons
    SimpleDraw::StaticTerminate();
    DebugUI::StaticTerminate();
    TextureManager::StaticTerminate();
    GraphicsSystem::StaticTerminate();
    InputSystem::StaticTerminate();

    //close the app
    myWindow.Terminate();
    LOG("App Ended");
}

void App::Quit()
{
    mRunning = false;
}

void TRengine::App::ChangeState(const std::string& stateName)
{
    auto iter = mAppStates.find(stateName);
    if (iter != mAppStates.end())
    {
        mNextState = iter->second.get();
    }
}
