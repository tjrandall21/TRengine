#include "Planet.h"

using namespace TRengine;
using namespace TRengine::Graphics;

void Planet::Initialize(std::filesystem::path texturePath, float radius, float orbitRadius, float orbitSpeed, float rotationSpeed)
{
    mTexturePath = texturePath;
    mRadius = radius;
    mOrbitRadius = orbitRadius;
    mOrbitSpeed = orbitSpeed;
    mRotationSpeed = rotationSpeed;
    mRotationPoint = Math::Vector3::Zero;
    MeshPX mesh = MeshBuilder::CreateSpherePX(30, 30, mRadius);
    mMeshBuffer.Initialize(mesh);
}

void Planet::LoadTexture()
{
    mTextureId = TextureManager::Get()->LoadTexture(mTexturePath);
}

void Planet::Terminate()
{
    TextureManager::Get()->ReleaseTexture(mTextureId);
    mMeshBuffer.Terminate();
}

void Planet::Render(Camera& camera, ConstantBuffer& constantBuffer)
{

    float x = std::cos(mOrbitRotation) * mOrbitRadius;
    float z = std::sin(mOrbitRotation) * mOrbitRadius;
    
    Math::Matrix4 matWorld = Math::Matrix4::RotationY(mRotation) * Math::Matrix4::Translation(x+mRotationPoint.x, mRotationPoint.y, z+mRotationPoint.z);
    Math::Matrix4 wvp = matWorld * camera.GetViewMatrix() * camera.GetProjectionMatrix();
    wvp = Math::Transpose(wvp);
    constantBuffer.Update(&wvp);

    TextureManager::Get()->BindPS(mTextureId, 0);

    mMeshBuffer.Render();
}

void Planet::DrawOrbit()
{
    SimpleDraw::AddGroundCircle(100, mOrbitRadius, Math::Vector3::Zero, Graphics::Colors::White);
}

void Planet::Update(float deltaTime)
{
    mOrbitRotation += mOrbitSpeed * deltaTime;
    mRotation += mRotationSpeed * deltaTime;
}

TRengine::Math::Vector3 Planet::GetPosition()
{
    return Math::Vector3(std::cos(mOrbitRotation) * mOrbitRadius,0, std::sin(mOrbitRotation) * mOrbitRadius);
}

TRengine::Math::Vector3 Planet::GetCameraOffset()
{
    return TRengine::Math::Vector3(0,mRadius*3,mRadius*4);
}

void Planet::SetRotationPoint(Math::Vector3 rotationPoint)
{
    mRotationPoint = rotationPoint;
}
