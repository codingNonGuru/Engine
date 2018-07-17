#include "SDL2/SDL.h"

#include "Model.hpp"
#include "ModelManager.hpp"
#include "RenderManager.hpp"
#include "Camera.hpp"
#include "InputHandler.hpp"

#include "Game/PregameScene.hpp"

PregameScene::PregameScene()
{
	camera_ = RenderManager::GetCamera("main");
}

void PregameScene::Update()
{
	if(InputHandler::IsPressed(SDL_SCANCODE_A))
	{
		camera_->Spin(0.01f);
	}
	else if(InputHandler::IsPressed(SDL_SCANCODE_D))
	{
		camera_->Spin(-0.01f);
	}
}

void PregameScene::Render()
{
	RenderManager::EnableDepthTesting();

	RenderManager::ClearDefaultBuffer();

	auto cubeModel = ModelManager::GetModel("Cube");
	cubeModel->Render(camera_);
}
