#pragma once

#include "Conventions.hpp"
#include "Delegate.hpp"

class Screen;
class RenderManager;
class Window;

class Engine
{
private:
	static bool isRunning_;

	static Screen* screen_;

	static Window* window_;

	static RenderManager* renderManager_;

public:
	static Delegate OnInitialize_;

	static void Initialize();

	static void StartGameLoop();

	static void ShutDown();

	static Window* GetWindow();

	static Screen* GetScreen();
};
