#pragma once

#include <TRengine/inc/TRengine.h>	


class Planet
{
public:
    void Initialize(std::filesystem::path texturePath, float radius, float orbitRadius, float orbitSpeed, float rotationSpeed = 1);
    void LoadTexture();
    void Terminate();
    void Render(TRengine::Graphics::Camera& camera, TRengine::Graphics::ConstantBuffer& constantBuffer);
    void DrawOrbit();
    void Update(float deltaTime);
    TRengine::Math::Vector3 GetPosition();
    TRengine::Math::Vector3 GetCameraOffset();
    void SetRotationPoint(Math::Vector3 rotationPoint);
    

private:
    TRengine::Graphics::MeshBuffer mMeshBuffer;
    TRengine::Graphics::TextureId mTextureId;
    std::filesystem::path mTexturePath;
    Math::Vector3 mRotationPoint;
    float mRotation = 0.0f;
    float mRotationSpeed = 1.0f;
    float mRadius = 0.0f;
    float mOrbitRotation = 0.0f;
    float mOrbitRadius = 0.0f;
    float mOrbitSpeed = 1.0f;
};

