#include "Precompiled.h"
#include "WindowMessageHandler.h"
#include "DebugUtil.h"

using namespace TRengine;
using namespace TRengine::Core;

void TRengine::Core::WindowMessageHandler::Hook(HWND window, Callback cb)
{
    mWindow = window;
    mPreviousCallback = (Callback)GetWindowLongPtrA(window, GWLP_WNDPROC);
    SetWindowLongPtrA(window, GWLP_WNDPROC, (LONG_PTR)cb);

}

void TRengine::Core::WindowMessageHandler::Unhook()
{
    SetWindowLongPtrA(mWindow, GWLP_WNDPROC, (LONG_PTR)mPreviousCallback);
    mWindow = nullptr;
}

LRESULT TRengine::Core::WindowMessageHandler::ForwardMessage(HWND window, UINT msg, WPARAM wParam, LPARAM lParam)
{
    ASSERT(mPreviousCallback != nullptr, "WindowMessageHandler: no callback is hooked");
    return CallWindowProcA((WNDPROC)mPreviousCallback, window, msg, wParam, lParam);
}
