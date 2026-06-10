#include "ShapeStates.h"

using namespace TRengine;
using namespace TRengine::Graphics;

void ShapeState::Initialize()
{
    CreateShape();
    mMeshBuffer.Initialize(mVertices.data(), sizeof(VertexPC), mVertices.size());

    std::filesystem::path shaderPath = L"../../Assets/Shaders/DoColor.fx";
    mVertexShader.Initialize<VertexPC>(shaderPath);
    mPixelShader.Initialize(shaderPath);
}

void ShapeState::Terminate()
{
    mVertices.clear();
    mPixelShader.Terminate();
    mVertexShader.Terminate();
    mMeshBuffer.Terminate();
}

void ShapeState::Update(float deltaTime)
{
}

void ShapeState::Render()
{
    mVertexShader.Bind();
    mPixelShader.Bind();

    mMeshBuffer.Render();
}

void ShapeState::CreateShape()
{
    mVertices.push_back({ {-0.5f,-0.5f, 0.0f }, {TRengine::Graphics::Colors::Blue} });
    mVertices.push_back({ {0.0f,0.5f, 0.0f}, {TRengine::Graphics::Colors::Orange} });
    mVertices.push_back({ {0.5f,-0.5f, 0.0f}, {TRengine::Graphics::Colors::Red} });
}
