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

	mCharacter.Initialize(L"Elvis/Elvis.model");
	mCharacter.transform.position = { 0.0f, 0.0f, 0.0f };

	std::filesystem::path shaderPath = L"../../Assets/Shaders/Standard.fx";
	mStandardEffect.Initialize(shaderPath);
	mStandardEffect.SetCamera(mCamera);
	mStandardEffect.SetDirecitonalLight(mDirectionalLight);
}

void GameState::Terminate()
{
	mStandardEffect.Terminate();
	mCharacter.Terminate();
}

void GameState::Update(float deltaTime)
{
	UpdateCamera(deltaTime);
}

void GameState::Render()
{
	SimpleDraw::AddGroundPlane(20.0f, Colors::LightGray);
	SimpleDraw::Render(mCamera);

	StandardEffect* effect = &mStandardEffect;
	effect->Begin();
		effect->Render(mCharacter);
	effect->End();


	mStandardEffect.Begin();
		mStandardEffect.Render(mCharacter);
	mStandardEffect.End();

}


void GameState::DebugUI()
{
	ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::DragFloat3("Direction", &mDirectionalLight.direction.x, 0.01f))
		{
			mDirectionalLight.direction = Math::Normalize(mDirectionalLight.direction);
		}
		ImGui::ColorEdit4("Ambient##Light", &mDirectionalLight.ambient.r);
		ImGui::ColorEdit4("Diffuse##Light", &mDirectionalLight.diffuse.r);
		ImGui::ColorEdit4("Specular##Light", &mDirectionalLight.specular.r);
	}
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