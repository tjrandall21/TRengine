#include "GameState.h"

using namespace TRengine;
using namespace TRengine::Graphics;
using namespace TRengine::Input;


void GameState::Initialize()
{
	mCamera.SetPosition({ 0.0f, 20.0f, -30.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	//initialize planets
	Planet sun = Planet();
	sun.Initialize("planets/sun.jpg", 5.0f, 0.0f, 0.0f,0.0f);
	mPlanets.push_back(sun);
	mPlanets.push_back(Planet());
	mPlanets[1].Initialize("planets/mercury.jpg", 0.4f, 6.0f, 2.0f, 0.5f);
	mPlanets.push_back(Planet());
	mPlanets[2].Initialize("planets/venus.jpg", 0.9f, 8.0f, 1.5f, 0.1f);
	mPlanets.push_back(Planet());
	mPlanets[3].Initialize("earth.jpg", 1.0f, 11.0f, 1.0f);
	mPlanets.push_back(Planet());
	mPlanets[4].Initialize("planets/mars.jpg", 0.8f, 13.0f, 0.9f);
	mPlanets.push_back(Planet());
	mPlanets[5].Initialize("planets/jupiter.jpg", 2.5f, 17.0f, 0.6f, 1.8f);
	mPlanets.push_back(Planet());
	mPlanets[6].Initialize("planets/saturn.jpg", 2.0f, 23.0f, 0.5f,1.6f);
	mPlanets.push_back(Planet());
	mPlanets[7].Initialize("planets/uranus.jpg", 1.2f, 26.0f, 0.4f,1.2f);
	mPlanets.push_back(Planet());
	mPlanets[8].Initialize("planets/neptune.jpg", 1.2f, 29.0f, 0.3f,1.2f);
	mPlanets.push_back(Planet());
	mPlanets[9].Initialize("planets/pluto.jpg", 0.3f, 32.0f, 0.2f, 0.1f);
	mPlanets.push_back(Planet());
	mPlanets[10].Initialize("misc/concrete.jpg", 0.1f, 2.0f, 3.0f);

	MeshPX mesh = MeshBuilder::CreateSkySpherePX(30, 30, 300);
	mSkyMeshBuffer.Initialize(mesh);

	std::filesystem::path shaderPath = L"../../Assets/Shaders/DoTexture.fx";
	mVertexShader.Initialize<VertexPX>(shaderPath);
	mPixelShader.Initialize(shaderPath);
	mConstantBuffer.Initialize(sizeof(Math::Matrix4));

	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);

	//load textures
	for (Planet& planet : mPlanets)
	{
		planet.LoadTexture();
	}

	mSkyTextureId = TextureManager::Get()->LoadTexture("skysphere/space.jpg");
}

void GameState::Terminate()
{
	TextureManager::Get()->ReleaseTexture(mSkyTextureId);
	mSkyMeshBuffer.Terminate();

	for (Planet& planet : mPlanets)
	{
		planet.Terminate();
	}
	
	mSampler.Terminate();
	mConstantBuffer.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
}

void GameState::Update(float deltaTime)
{
	for (Planet& planet : mPlanets)
	{
		planet.Update(deltaTime);
	}
	mPlanets[10].SetRotationPoint(mPlanets[3].GetPosition());
	UpdateCamera(deltaTime);
}
int gFocusedPlanetIndex = -1;
bool gShowOrbitLines = false;

void GameState::Render()
{
	mVertexShader.Bind();
	mPixelShader.Bind();

	mConstantBuffer.BindVS(0);
	mSampler.BindPS(0);	

	for (Planet& planet : mPlanets)
	{
		planet.Render(mCamera,mConstantBuffer);
	}

	//skysphere
	Math::Matrix4 matWorld = Math::Matrix4::Identity;
	Math::Matrix4 wvp = matWorld * mCamera.GetViewMatrix() * mCamera.GetProjectionMatrix();
	wvp = Math::Transpose(wvp);
	mConstantBuffer.Update(&wvp);

	TextureManager::Get()->BindPS(mSkyTextureId, 0);

	mSkyMeshBuffer.Render();

	if (gShowOrbitLines)
	{
		for (Planet& planet : mPlanets)
		{
			planet.DrawOrbit();
		}
	}

	SimpleDraw::Render(mCamera);
}



const char* gPlanetNames[] =
{
	"None",
	"Sun",
	"Mercury",
	"Venus",
	"Earth",
	"Mars",
	"Jupiter",
	"Saturn",
	"Uranus",
	"Neptune",
	"Pluto"
};

void GameState::DebugUI()
{
	ImGui::Begin("Solar System", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	int currentPlanet = gFocusedPlanetIndex+1;
	if (ImGui::Combo("Planet", &currentPlanet, gPlanetNames, std::size(gPlanetNames)))
	{
		gFocusedPlanetIndex = currentPlanet-1;
	}
	ImGui::Checkbox("Orbit Lines", &gShowOrbitLines);

	ImGui::End();
}

void GameState::UpdateCamera(float deltaTime)
{
	if (gFocusedPlanetIndex == -1)
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
	else
	{
		Math::Vector3 planetPos = mPlanets[gFocusedPlanetIndex].GetPosition();

		mCamera.SetPosition(planetPos + mPlanets[gFocusedPlanetIndex].GetCameraOffset());
		mCamera.SetLookAt(planetPos);
	}
}