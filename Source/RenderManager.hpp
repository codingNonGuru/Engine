#pragma once

#include "Conventions.hpp"
#include "Utility/Color.hpp"

class Camera;
class Window;

class RenderManager
{
private:
	static Window* window_;

	static Map <Camera*> cameras_;

	static Color backgroundColor_;

public:
	static void Initialize();

	static void Update();

	static void UpdateCameras();

	static void EnableDepthTesting();

	static void DisableDepthTesting();

	static void SetBlendMode();

	static void DisableBlending();

	static Camera* GetCamera(Word);

	static void AddCamera(Word, Camera*);

	static Camera* GetInterfaceCamera();

	static void ClearDefaultBuffer();

	static void SetBackgroundColor(Color);

	static const Size SHADOW_MAP_SIZE;

	static const float SHADOW_MAP_SIZE_MODIFIER;
};
