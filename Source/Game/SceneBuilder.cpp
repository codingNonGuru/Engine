#include "SceneManager.hpp"

#include "SceneBuilder.hpp"
#include "Game/MainScene.hpp"

void SceneBuilder::GenerateScenes()
{
	SceneManager::AddScene(new MainScene(), "MainScene");
}

