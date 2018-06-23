#include <GL/glew.h>
#include "SDL2/SDL.h"

#include "Engine.hpp"
#include "Screen.hpp"
#include "RenderManager.hpp"
#include "Window.hpp"
#include "InputHandler.hpp"
#include "Interface/Interface.hpp"
#include "Interface/Element.hpp"
#include "Time.hpp"

bool Engine::isRunning_ = false;

Window* Engine::window_ = nullptr;

Screen* Engine::screen_ = nullptr;

RenderManager* Engine::renderManager_ = nullptr;

void Engine::Initialize()
{
	screen_ = new Screen(2560, 1440);

	window_ = new Window(screen_);

	glewInit();

	glEnable(GL_DEBUG_OUTPUT);

	renderManager_ = RenderManager::Get();

	renderManager_->Initialize();
}

void Engine::StartGameLoop()
{
	isRunning_ = true;

	InputHandler::Initialize();

	while(isRunning_)
	{
		Time::Update();

		InputHandler::Update();

		if(InputHandler::IsPressed(SDLK_ESCAPE))
			isRunning_ = false;

		if(InputHandler::IsPressed(SDLK_a))
		{
			auto mainMenu = Interface::GetElement("MainMenu");
			mainMenu->Open();
		}

		Interface::Update();

		renderManager_->Update();
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
