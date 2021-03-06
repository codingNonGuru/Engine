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
#include "SceneManager.hpp"
#include "TaskManager.hpp"
#include "Time.hpp"
#include "Utility/Perlin.hpp"

bool Engine::isRunning_ = false;

Screen* Engine::screen_ = nullptr;

Delegate Engine::OnInitialize_ = Delegate();

Delegate Engine::OnGameLoopStart_ = Delegate();

Delegate Engine::OnShutDown_ = Delegate();

void Engine::Initialize()
{
	AssetManager::Initialize();

	screen_ = new Screen(2560, 1440);

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

		if(InputHandler::IsPressed(SDL_SCANCODE_ESCAPE))
		{
			ShutDown();
		}

		SceneManager::Update();

		TaskManager::Update();

		Interface::Update();

		RenderManager::Update();
	}
}

void Engine::ShutDown()
{
	isRunning_ = false;

	OnShutDown_.Invoke();

	SDL_Quit();
}

Screen* Engine::GetScreen()
{
	return screen_;
}
