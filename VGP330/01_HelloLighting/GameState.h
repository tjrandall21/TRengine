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
	TRengine::Graphics::RenderTarget mRenderTarget;

	TRengine::Graphics::RenderObject mRenderObject;
	TRengine::Graphics::RenderObject mRenderSphere;
	TRengine::Graphics::RenderObject mRenderSphere2;
	bool mUseCelShading = false;
};