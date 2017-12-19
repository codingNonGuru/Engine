#include "SDL2/SDL.h"

#include "InputHandler.hpp"

#include "Engine.hpp"
#include "Screen.hpp"

Mouse InputHandler::mouse_ = Mouse();

int InputHandler::keyCount_ = 0;

container::Array<int> InputHandler::currentKeys_ = container::Array<int>();

container::Array<int> InputHandler::formerKeys_ = container::Array<int>();

void InputHandler::Initialize()
{
	currentKeys_.Initialize(64);

	formerKeys_.Initialize(64);
}

void InputHandler::Update()
{
	formerKeys_.Reset();

	for(auto key = currentKeys_.GetStart(); key != currentKeys_.GetEnd(); ++key)
		*formerKeys_.Allocate() = *key;

	currentKeys_.Reset();

	SDL_PumpEvents();

	UpdateMouse();

	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
		if (event.type == SDL_KEYDOWN)
		{
			*currentKeys_.Allocate() = event.key.keysym.sym;
		}
		else if(event.type == SDL_MOUSEBUTTONDOWN)
		{
			if(event.button.button == SDL_BUTTON_LEFT)
				mouse_.CurrentLeft_ = true;
			else if(event.button.button == SDL_BUTTON_RIGHT)
				mouse_.CurrentRight_ = true;
		}
		else if(event.type == SDL_MOUSEWHEEL)
		{
			if(event.wheel.y < 0)
				mouse_.ScrollDown_ = true;
			else if(event.wheel.y > 0)
				mouse_.ScrollUp_ = true;
		}
	}
}

void InputHandler::UpdateMouse()
{
	mouse_.ScrollUp_ = false;
	mouse_.ScrollDown_ = false;

	mouse_.LastLeft_ = mouse_.CurrentLeft_;
	mouse_.LastRight_ = mouse_.CurrentRight_;

	mouse_.CurrentRight_ = false;
	mouse_.CurrentLeft_ = false;

	int x, y;
	unsigned int mouseFlags = SDL_GetMouseState(&x, &y);
	mouse_.Position_.x = x - Engine::GetScreen()->getWidthInteger() / 2;
	mouse_.Position_.y = y - Engine::GetScreen()->getHeightInteger() / 2;
}

const Mouse & InputHandler::GetMouse()
{
	return mouse_;
}

Position2 InputHandler::GetMousePosition()
{
	return mouse_.Position_;
}

bool InputHandler::IsPressed(int32_t key)
{
	for(auto currKey = currentKeys_.GetStart(); currKey != currentKeys_.GetEnd(); ++currKey)
		if(*currKey == key)
			return true;

	return false;
}

bool InputHandler::WasPressed(int32_t key)
{
	int i = 0;
	for(auto formKey = formerKeys_.GetStart(); formKey != formerKeys_.GetEnd(); ++formKey, ++i)
	{
		if(*formKey == key && *formKey == *(currentKeys_.GetStart() + i))
			return true;
		else if(*formKey == key)
			return false;
	}

	return false;
}
