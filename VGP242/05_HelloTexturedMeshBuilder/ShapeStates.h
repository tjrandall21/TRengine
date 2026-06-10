#pragma once
#include <TRengine/inc/TRengine.h>

class ShapeState : public TRengine::AppState
{
public:
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void Render() override;
protected:
    virtual void CreateShape();

    TRengine::Graphics::TextureId mTextureId;
    TRengine::Graphics::MeshPX mMesh;

    TRengine::Graphics::MeshBuffer mMeshBuffer;
    TRengine::Graphics::VertexShader mVertexShader;
    TRengine::Graphics::PixelShader mPixelShader;
    TRengine::Graphics::Sampler mSampler;

    TRengine::Graphics::Camera mCamera;
    TRengine::Graphics::ConstantBuffer mConstantBuffer;
};