#pragma once

#include "Conventions.hpp"

class Screen;
class SDL_Window;

class Engine
{
private:
	static bool isRunning_;

	static SDL_Window* window_;

	static Screen* screen_;

public:
	static void Initialize();

	static void StartGameLoop();

	static void ShutDown();
};
