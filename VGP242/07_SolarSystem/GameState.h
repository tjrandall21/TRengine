#pragma once

#include <TRengine/inc/TRengine.h>	
#include "Planet.h"

class GameState : public TRengine::AppState
{
public:
	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

private:
	void UpdateCamera(float deltaTime);
	TRengine::Graphics::Camera mCamera;

	TRengine::Graphics::MeshBuffer mSkyMeshBuffer;
	TRengine::Graphics::TextureId mSkyTextureId;

	std::vector<Planet> mPlanets = {};

	TRengine::Graphics::VertexShader mVertexShader;
	TRengine::Graphics::PixelShader mPixelShader;
	TRengine::Graphics::Sampler mSampler;

	TRengine::Graphics::ConstantBuffer mConstantBuffer;
};