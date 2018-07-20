#include "RenderManager.hpp"
#include "InputHandler.hpp"
#include "Camera.hpp"

#include "Game/WorldScene.hpp"
#include "Game/World.hpp"
#include "Game/Game.hpp"
#include "Game/TerrainModel.hpp"
#include "Game/Types.hpp"

WorldScene* WorldScene::instance_ = nullptr;

WorldScene::WorldScene()
{
	if(instance_ != nullptr)
		return;

	instance_ = this;

	Game::OnStartGame_.Add(this, &WorldScene::HandleStartGame);

	world_ = nullptr;

	worldModel_ = nullptr;

	camera_ = RenderManager::GetCamera(Cameras::WORLD);
}

void WorldScene::Initialize(File* file)
{
}

void WorldScene::Initialize(const WorldParameterSet& parameterSet)
{
	world_ = new World(parameterSet);
}

void WorldScene::Update()
{
	if(InputHandler::IsPressed(SDL_SCANCODE_A))
	{
		camera_->Spin(-0.01f);
	}
	else if(InputHandler::IsPressed(SDL_SCANCODE_D))
	{
		camera_->Spin(0.01f);
	}

	auto viewDistance = camera_->GetViewDistance();
	if(InputHandler::IsPressed(SDL_SCANCODE_W))
	{
		camera_->PushForward(-viewDistance * 0.01f);
	}
	else if(InputHandler::IsPressed(SDL_SCANCODE_S))
	{
		camera_->PushForward(viewDistance * 0.01f);
	}

	auto & mouse = InputHandler::GetMouse();
	if(mouse.ScrollUp_)
	{
		camera_->Zoom(0.05f);
	}
	else if(mouse.ScrollDown_)
	{
		camera_->Zoom(-0.05f);
	}
}

void WorldScene::Render()
{
	RenderManager::EnableDepthTesting();

	if(worldModel_ != nullptr)
	{
		worldModel_->Render(camera_);
	}
}

World* WorldScene::GetWorld()
{
	if(instance_ == nullptr)
		return nullptr;

	return instance_->world_;
}

void WorldScene::HandleStartGame()
{
	if(worldModel_ == nullptr)
	{
		worldModel_ = new TerrainModel();
	}
}
