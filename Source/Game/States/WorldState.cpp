#include "Scene.hpp"
#include "SceneManager.hpp"
#include "TaskManager.hpp"
#include "Interface/Interface.hpp"
#include "Interface/Element.hpp"

#include "Game/States/WorldState.hpp"
#include "Game/Types.hpp"

WorldState::WorldState()
{
	worldScene_ = SceneManager::Get(Scenes::WORLD);

	topBar_ = Interface::GetElement(Elements::TOP_BAR);

	topPanel_ = Interface::GetElement(Elements::TOP_PANEL);
}

void WorldState::HandleEnter()
{
	TaskManager::Add()->Initialize(this, &WorldState::HandleSceneEnable, 2.0f);
}

void WorldState::HandleExit()
{
	if(worldScene_ != nullptr)
	{
		worldScene_->Disable();
	}

	topBar_->Close();

	topPanel_->Close();
}

void WorldState::HandleSceneEnable()
{
	if(worldScene_ != nullptr)
	{
		worldScene_->Enable();
	}

	topBar_->Open();

	topPanel_->Open();
}
