#include "Game/MainScene.hpp"
#include "Game/World.hpp"

void MainScene::Initialize(File* file)
{
}

void MainScene::Initialize(const WorldParameterSet& parameterSet)
{
	world_ = new World(parameterSet);
}
