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

	Mesh mesh = MeshBuilder::CreateSphere(50, 50, 1.0f);
	Mesh mesh2 = MeshBuilder::CreateSphere(50, 50, 1.2f);
	Mesh mesh3 = MeshBuilder::CreateSphere(150, 150, 1.0f);

	mRenderObject.meshBuffer.Initialize(mesh);
	TextureManager* tm = TextureManager::Get();
	mRenderObject.diffuseMapId = tm->LoadTexture("earth.jpg");
	mRenderObject.specMapId = tm->LoadTexture("earth_spec.jpg");
	mRenderObject.normMapId = tm->LoadTexture("earth_normal.jpg");
	mRenderObject.bumpMapId = tm->LoadTexture("earth_bump.jpg");
	mRenderObject.transform.position = { -5,0,0 };

	mRenderSphere.meshBuffer.Initialize(mesh2);
	mRenderSphere.diffuseMapId = tm->LoadTexture("lava/lava-and-rock_albedo.png");
	mRenderSphere.specMapId = tm->LoadTexture("lava/lava-and-rock_roughness.png");
	mRenderSphere.normMapId = tm->LoadTexture("lava/lava-and-rock_normal-ogl.png");
	mRenderSphere.bumpMapId = tm->LoadTexture("lava/lava-and-rock_height.png");

	mRenderSphere2.meshBuffer.Initialize(mesh3);
	mRenderSphere2.diffuseMapId = tm->LoadTexture("brick/grime-alley-bricks_2_albedo.png");
	mRenderSphere2.specMapId = tm->LoadTexture("brick/grime-alley-bricks_2_roughness.png");
	mRenderSphere2.normMapId = tm->LoadTexture("brick/grime-alley-bricks_2_normal-ogl.png");
	mRenderSphere2.bumpMapId = tm->LoadTexture("brick/grime-alley-bricks_2_height.png");
	mRenderSphere2.transform.position = { 5,0,0 };

	std::filesystem::path shaderPath = L"../../Assets/Shaders/Standard.fx";
	mStandardEffect.Initialize(shaderPath);
	mStandardEffect.SetCamera(mCamera);
	mStandardEffect.SetDirecitonalLight(mDirectionalLight);

	std::filesystem::path shaderPathCel = L"../../Assets/Shaders/CelShader.fx";
	mStandardEffectCel.Initialize(shaderPathCel);
	mStandardEffectCel.SetCamera(mCamera);
	mStandardEffectCel.SetDirecitonalLight(mDirectionalLight);

	const uint32_t size = 512;
	mRenderTarget.Initialize(size, size, RenderTarget::Format::RGBA_U32);
}

void GameState::Terminate()
{
	mRenderTarget.Terminate();
	mStandardEffect.Terminate();
	mStandardEffectCel.Terminate();
	mRenderObject.Terminate();
	mRenderSphere.Terminate();
	mRenderSphere2.Terminate();
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
		effect->Render(mRenderSphere);
		effect->Render(mRenderSphere2);
	effect->End();

	mRenderTarget.BeginRender();
		mStandardEffect.Begin();
			mStandardEffect.Render(mRenderObject);
			mStandardEffect.Render(mRenderSphere);
			mStandardEffect.Render(mRenderSphere2);
		mStandardEffect.End();
	mRenderTarget.EndRender();
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
	if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::ColorEdit4("Emissive##Material", &mRenderObject.material.emissive.r);
		ImGui::ColorEdit4("Ambient##Material", &mRenderObject.material.ambient.r);
		ImGui::ColorEdit4("Diffuse##Material", &mRenderObject.material.diffuse.r);
		ImGui::ColorEdit4("Specular##Material", &mRenderObject.material.specular.r);
		ImGui::DragFloat("Power##Material", &mRenderObject.material.shininess, 0.01f, 0.0f, 1000.0f);
	}
	ImGui::Checkbox("UseCelShading", &mUseCelShading);
	StandardEffect* effect = mUseCelShading ? &mStandardEffectCel : &mStandardEffect;
		effect->DebugUI();
	ImGui::Image(
		mRenderTarget.GetRawData(),
		{128,128},
		{0,0},
		{1,1},
		{1,1,1,1},
		{1,1,1,1});
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