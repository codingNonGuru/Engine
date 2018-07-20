#include "Scene.hpp"
#include "SceneManager.hpp"
#include "TaskManager.hpp"

#include "Game/States/WorldState.hpp"
#include "Game/Types.hpp"

WorldState::WorldState()
{
	worldScene_ = SceneManager::Get(Scenes::WORLD);
}

void WorldState::HandleEnter()
{
	TaskManager::Add()->Initialize(this, &WorldState::HandleSceneEnable, 0.5f);
}

void WorldState::HandleExit()
{
	if(worldScene_ != nullptr)
	{
		worldScene_->Disable();
	}
}

void WorldState::HandleSceneEnable()
{
	if(worldScene_ != nullptr)
	{
		worldScene_->Enable();
	}
}
