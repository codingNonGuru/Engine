#pragma once

#include "Conventions.hpp"

class Screen;
class Camera;

class RenderManager
{
private:
	static RenderManager* instance_;

	Screen* screen_;

	Map <Camera, LongWord> cameras_;

public:
	static RenderManager* Get();

	void Initialize();

	void Update();

	void UpdateCameras();

	void EnableDepthTesting();

	void DisableDepthTesting();

	void SetBlendMode();

	void DisableBlending();
};
