#pragma once

#include "Conventions.hpp"

class Scene;

class SceneManager
{
	static Map <Scene*, LongWord> scenes_;

public:
	static Scene * GetScene(const char*);

	static Scene * AddScene(Scene*, const char*);

	static void Update();

	static void Render();
};
