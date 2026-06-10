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
};