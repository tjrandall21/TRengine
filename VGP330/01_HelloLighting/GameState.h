#pragma once

#include <TRengine/inc/TRengine.h>	

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
	TRengine::Graphics::DirectionalLight mDirectionalLight;
	TRengine::Graphics::StandardEffect mStandardEffect;
	TRengine::Graphics::StandardEffect mStandardEffectCel;

	TRengine::Graphics::RenderObject mRenderObject;
	bool mUseCelShading = false;
};