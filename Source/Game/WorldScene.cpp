#include "Game/WorldScene.hpp"
#include "Game/World.hpp"
#include "Game/Game.hpp"
#include "Game/TerrainModel.hpp"

WorldScene* WorldScene::instance_ = nullptr;

WorldScene::WorldScene()
{
	if(instance_ != nullptr)
		return;

	instance_ = this;

	Game::OnStartGame_.Add(this, &WorldScene::HandleStartGame);

	world_ = nullptr;

	worldModel_ = nullptr;
}

void WorldScene::Initialize(File* file)
{
}

void WorldScene::Initialize(const WorldParameterSet& parameterSet)
{
	world_ = new World(parameterSet);
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
