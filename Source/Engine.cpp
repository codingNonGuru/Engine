#include <GL/glew.h>
#include "SDL2/SDL.h"

#include "Engine.hpp"
#include "Screen.hpp"
#include "RenderManager.hpp"
#include "Window.hpp"
#include "InputHandler.hpp"
#include "AssetManager.hpp"
#include "Interface/Interface.hpp"
#include "Interface/Element.hpp"
#include "Time.hpp"
#include "Utility/Perlin.hpp"
#include "TaskManager.hpp"

bool Engine::isRunning_ = false;

Window* Engine::window_ = nullptr;

Screen* Engine::screen_ = nullptr;

Delegate Engine::OnInitialize_ = Delegate();

Delegate Engine::OnGameLoopStart_ = Delegate();

void Engine::Initialize()
{
	AssetManager::Initialize();

	screen_ = new Screen(2560, 1440);

	window_ = new Window(screen_);

	RenderManager::Initialize();

	AssetManager::LoadAssets();

	InputHandler::Initialize();

	OnInitialize_.Invoke();
}

void Engine::StartGameLoop()
{
	isRunning_ = true;

	Time::Update();

	OnGameLoopStart_.Invoke();

	while(isRunning_)
	{
		Time::Update();

		InputHandler::Update();

		if(InputHandler::IsPressed(SDLK_ESCAPE))
		{
			ShutDown();
			break;
		}

		TaskManager::Update();

		Interface::Update();

		RenderManager::Update();
	}
}

void Engine::ShutDown()
{
	isRunning_ = false;

	SDL_Quit();
}

Window* Engine::GetWindow()
{
	return window_;
}

Screen* Engine::GetScreen()
{
	return screen_;
}
