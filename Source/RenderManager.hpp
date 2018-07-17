#pragma once

#include "Conventions.hpp"
#include "Utility/Color.hpp"

class Screen;
class Camera;

class RenderManager
{
private:
	static Map <Camera, LongWord> cameras_;

	static Color backgroundColor_;

public:
	static void Initialize();

	static void Update();

	static void UpdateCameras();

	static void EnableDepthTesting();

	static void DisableDepthTesting();

	static void SetBlendMode();

	static void DisableBlending();

	static Camera* GetCamera(LongWord);
};
