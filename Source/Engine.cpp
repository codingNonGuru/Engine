#include <GL/glew.h>
#include "SDL2/SDL.h"

#include "Engine.hpp"
#include "Screen.hpp"

bool Engine::isRunning_ = false;

SDL_Window* Engine::window_ = nullptr;

Screen* Engine::screen_ = nullptr;

void Engine::Initialize()
{
	screen_ = new Screen();
	screen_->Initialize(2560, 1440);

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 5);

	SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_BUFFER_SIZE, 32 );
	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 24 );
	SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

	SDL_GL_SetAttribute( SDL_GL_MULTISAMPLEBUFFERS, 1 );
	SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, 8 );

	window_ = SDL_CreateWindow("Engine", 0, 0, screen_->getWidthInteger(), screen_->getHeightInteger(), SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN);
	SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
	SDL_GLContext context = SDL_GL_CreateContext(window_);
	SDL_GL_MakeCurrent(window_, context);

	SDL_GL_SetSwapInterval(1);

	glewInit();

	glViewport(0, 0, screen_->getWidthInteger(), screen_->getHeightInteger());
}

void Engine::StartGameLoop()
{
	isRunning_ = true;

	while(isRunning_)
	{

	}
}

void Engine::ShutDown()
{
	isRunning_ = false;
	SDL_Quit();
}
