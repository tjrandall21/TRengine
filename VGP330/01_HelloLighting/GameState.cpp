#include "GameState.h"

using namespace TRengine;
using namespace TRengine::Graphics;
using namespace TRengine::Input;


void GameState::Initialize()
{
	mCamera.SetPosition({ 0.0f, 3.0f, -5.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	mDirectionalLight.direction = Math::Normalize({ 1.0f,-1.0f,1.0f });
	mDirectionalLight.ambient = { 0.4f,0.4f,0.4f,1.0f };
	mDirectionalLight.diffuse = { 0.7f,0.7f,0.7f,1.0f };
	mDirectionalLight.specular = { 0.9f,0.9f,0.9f,1.0f };

	Mesh mesh = MeshBuilder::CreateSphere(30, 30, 1.0f);
	mRenderObject.meshBuffer.Initialize(mesh);
	TextureManager* tm = TextureManager::Get();
	mRenderObject.diffuseMapId = tm->LoadTexture("earth.jpg");
	mRenderObject.specMapId = tm->LoadTexture("earth_spec.jpg");
	mRenderObject.normMapId = tm->LoadTexture("earth_normal.jpg");
	mRenderObject.bumpMapId = tm->LoadTexture("earth_bump.jpg");

	std::filesystem::path shaderPath = L"../../Assets/Shaders/Standard.fx";
	mStandardEffect.Initialize(shaderPath);
	mStandardEffect.SetCamera(mCamera);
	mStandardEffect.SetDirecitonalLight(mDirectionalLight);

	std::filesystem::path shaderPathCel = L"../../Assets/Shaders/CelShader.fx";
	mStandardEffectCel.Initialize(shaderPathCel);
	mStandardEffectCel.SetCamera(mCamera);
	mStandardEffectCel.SetDirecitonalLight(mDirectionalLight);


}

void GameState::Terminate()
{
	mStandardEffect.Terminate();
	mStandardEffectCel.Terminate();
	mRenderObject.Terminate();
}

void GameState::Update(float deltaTime)
{
	UpdateCamera(deltaTime);
}

void GameState::Render()
{
	SimpleDraw::AddGroundPlane(20.0f, Colors::LightGray);
	SimpleDraw::Render(mCamera);

	StandardEffect* effect = mUseCelShading ? &mStandardEffectCel : &mStandardEffect;
	effect->Begin();
		effect->Render(mRenderObject);
	effect->End();
}


void GameState::DebugUI()
{
	ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Checkbox("UseCelShading", &mUseCelShading);
		mStandardEffect.DebugUI();
	ImGui::End();
}

void GameState::UpdateCamera(float deltaTime)
{
	InputSystem* input = InputSystem::Get();
	const float moveSpeed = input->IsKeyDown(KeyCode::LSHIFT) ? 5.0f : 1.0f;
	const float turnSpeed = 0.2f;

	if (input->IsKeyDown(KeyCode::W))
	{
		mCamera.Walk(moveSpeed * deltaTime);
	}
	if (input->IsKeyDown(KeyCode::S))
	{
		mCamera.Walk(-moveSpeed * deltaTime);
	}
	if (input->IsKeyDown(KeyCode::D))
	{
		mCamera.Strafe(moveSpeed * deltaTime);
	}
	if (input->IsKeyDown(KeyCode::A))
	{
		mCamera.Strafe(-moveSpeed * deltaTime);
	}
	if (input->IsKeyDown(KeyCode::E))
	{
		mCamera.Rise(moveSpeed * deltaTime);
	}
	if (input->IsKeyDown(KeyCode::Q))
	{
		mCamera.Rise(-moveSpeed * deltaTime);
	}
	if (input->IsMouseDown(MouseButton::RBUTTON))
	{
		mCamera.Yaw(input->GetMouseMoveX() * turnSpeed * deltaTime);
		mCamera.Pitch(input->GetMouseMoveY() * turnSpeed * deltaTime);
	}
}