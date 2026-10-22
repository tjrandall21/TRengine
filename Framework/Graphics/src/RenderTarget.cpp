#include "Precompiled.h"
#include "RenderTarget.h"

using namespace TRengine;
using namespace TRengine::Graphics;

namespace
{
    DXGI_FORMAT GetDXGIFormat(RenderTarget::Format format)
    {
        switch (format)
        {
        case RenderTarget::Format::RGBA_U8: return DXGI_FORMAT_R8G8B8A8_UNORM;
        case RenderTarget::Format::RGBA_U32: return DXGI_FORMAT_R32G32B32A32_UINT;
        default:
            ASSERT(false, "RenderTarget: Unsupported format");
            break;
        }
        return DXGI_FORMAT_R8G8B8A8_UNORM;
    }
}

RenderTarget::~RenderTarget()
{
    ASSERT(mRenderTargetView == nullptr && mDepthStencilView == nullptr, "RenderTarget: must call terminate");
}
void RenderTarget::Initialize(uint32_t width, uint32_t height, Format format)
{
    D3D11_TEXTURE2D_DESC desc();
    //continue here
}
void RenderTarget::Terminate()
{
}
void RenderTarget::BeginRender(Color clearColor)
{
}