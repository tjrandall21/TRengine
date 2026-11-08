#include "Precompiled.h"
#include "StandardEffect.h"
#include "Camera.h"
#include "VertexTypes.h" 
#include "RenderObject.h"

void TRengine::Graphics::StandardEffect::Initialize(const std::filesystem::path& shaderpath)
{
    mTransformBuffer.Initialize();
    mLightBuffer.Initialize();
    mMaterialBuffer.Initialize();
    mSettingsBuffer.Initialize();

    mVertexShader.Initialize<Vertex>(shaderpath);
    mPixelShader.Initialize(shaderpath);
    mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);


}

void TRengine::Graphics::StandardEffect::Terminate()
{
    mSampler.Terminate();
    mPixelShader.Terminate();
    mVertexShader.Terminate();
    mMaterialBuffer.Terminate();
    mSettingsBuffer.Terminate();
    mLightBuffer.Terminate();
    mTransformBuffer.Terminate();
}

void TRengine::Graphics::StandardEffect::Begin()
{
    mVertexShader.Bind();
    mPixelShader.Bind();
    mSampler.BindVS(0);
    mSampler.BindPS(0);

    mTransformBuffer.BindVS(0);
    mLightBuffer.BindVS(1);
    mLightBuffer.BindPS(1);
    mMaterialBuffer.BindPS(2);
    mSettingsBuffer.BindVS(3);
    mSettingsBuffer.BindPS(3);
}

void TRengine::Graphics::StandardEffect::End()
{

}

void TRengine::Graphics::StandardEffect::Render(const RenderObject& renderObject)
{
    const Math::Matrix4 matWorld = renderObject.transform.GetMatrix();
    const Math::Matrix4 matView = mCamera->GetViewMatrix();
    const Math::Matrix4 matProj = mCamera->GetProjectionMatrix();
    const Math::Matrix4 matFinal = matWorld * matView * matProj;

    TransformData data;
    data.wvp = Math::Transpose(matFinal);
    data.world = Math::Transpose(matWorld);
    data.viewPosition = mCamera->GetPosition();
    mTransformBuffer.Update(data);
    mLightBuffer.Update(*mDirectionalLight);
    mMaterialBuffer.Update(renderObject.material);

    SettingsData settings;
    settings.useDiffuseMap = (mSettingsData.useDiffuseMap > 0 && renderObject.diffuseMapId > 0) ? 1 : 0;
    settings.useSpecMap = (mSettingsData.useSpecMap > 0 && renderObject.specMapId > 0) ? 1 : 0;
    settings.useNormMap = (mSettingsData.useNormMap > 0 && renderObject.normMapId > 0) ? 1 : 0;
    settings.useBumpMap = (mSettingsData.useBumpMap > 0 && renderObject.bumpMapId > 0) ? 1 : 0;
    settings.bumpWeight = mSettingsData.bumpWeight;
    mSettingsBuffer.Update(settings);

    TextureManager* tm = TextureManager::Get();
    tm->BindPS(renderObject.diffuseMapId, 0);
    tm->BindPS(renderObject.specMapId, 1);
    tm->BindPS(renderObject.normMapId, 2);
    tm->BindVS(renderObject.bumpMapId, 3);
    renderObject.meshBuffer.Render();
}

void TRengine::Graphics::StandardEffect::Render(const RenderGroup& renderGroup)
{
    const Math::Matrix4 matWorld = renderGroup.transform.GetMatrix();
    const Math::Matrix4 matView = mCamera->GetViewMatrix();
    const Math::Matrix4 matProj = mCamera->GetProjectionMatrix();
    const Math::Matrix4 matFinal = matWorld * matView * matProj;

    TransformData data;
    data.wvp = Math::Transpose(matFinal);
    data.world = Math::Transpose(matWorld);
    data.viewPosition = mCamera->GetPosition();
    mTransformBuffer.Update(data);
    mLightBuffer.Update(*mDirectionalLight);

    TextureManager* tm = TextureManager::Get();
    SettingsData settings;
    for (const RenderObject& renderObject : renderGroup.renderObjects)
    {
        mMaterialBuffer.Update(renderObject.material);
        settings.useDiffuseMap = (mSettingsData.useDiffuseMap > 0 && renderObject.diffuseMapId > 0) ? 1 : 0;
        settings.useSpecMap = (mSettingsData.useSpecMap > 0 && renderObject.specMapId > 0) ? 1 : 0;
        settings.useNormMap = (mSettingsData.useNormMap > 0 && renderObject.normMapId > 0) ? 1 : 0;
        settings.useBumpMap = (mSettingsData.useBumpMap > 0 && renderObject.bumpMapId > 0) ? 1 : 0;
        settings.bumpWeight = mSettingsData.bumpWeight;
        mSettingsBuffer.Update(settings);

        tm->BindPS(renderObject.diffuseMapId, 0);
        tm->BindPS(renderObject.specMapId, 1);
        tm->BindPS(renderObject.normMapId, 2);
        tm->BindVS(renderObject.bumpMapId, 3);
        renderObject.meshBuffer.Render();
    }
}

void TRengine::Graphics::StandardEffect::DebugUI()
{
    if (ImGui::CollapsingHeader("StandardEffect", ImGuiTreeNodeFlags_DefaultOpen))
    {
        bool useDiffuse = mSettingsData.useDiffuseMap > 0;
        if (ImGui::Checkbox("UseDiffuse", &useDiffuse))
        {
            mSettingsData.useDiffuseMap = useDiffuse ? 1 : 0;
        }
        bool useSpec = mSettingsData.useSpecMap > 0;
        if (ImGui::Checkbox("UseSpec", &useSpec))
        {
            mSettingsData.useSpecMap = useSpec ? 1 : 0;
        }
        bool useNorm = mSettingsData.useNormMap > 0;
        if (ImGui::Checkbox("UseNorm", &useNorm))
        {
            mSettingsData.useNormMap = useNorm ? 1 : 0;
        }
        bool useBump = mSettingsData.useBumpMap > 0;
        if (ImGui::Checkbox("UseBump", &useBump))
        {
            mSettingsData.useBumpMap = useBump ? 1 : 0;
        }
        ImGui::DragFloat("BumpWeight", &mSettingsData.bumpWeight, 0.01f, 0.0f, 100.0f);
    }
}

void TRengine::Graphics::StandardEffect::SetCamera(const Camera& camera)
{
    mCamera = &camera;
}

void TRengine::Graphics::StandardEffect::SetDirecitonalLight(const DirectionalLight& directionalLight)
{
    mDirectionalLight = &directionalLight;
}
