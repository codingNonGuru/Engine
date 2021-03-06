#include "SceneManager.hpp"

#include "Game/SceneBuilder.hpp"
#include "Game/WorldScene.hpp"
#include "Game/PregameScene.hpp"
#include "Game/Types.hpp"

void SceneBuilder::GenerateScenes()
{
	SceneManager::AddScene(new PregameScene(), Scenes::PREGAME);

	SceneManager::AddScene(new WorldScene(), Scenes::WORLD);
}

