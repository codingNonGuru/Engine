#include "Interface/Interface.hpp"
#include "Interface/Element.hpp"
#include "TaskManager.hpp"

#include "Game/States/PregameState.hpp"
#include "Game/Types.hpp"

PregameState::PregameState()
{
	mainMenu_ = Interface::GetElement(Elements::MAIN_MENU);

	newWorldMenu_ = Interface::GetElement(Elements::NEW_WORLD_MENU);

	worldPreviewPanel_ = Interface::GetElement(Elements::WORLD_PREVIEW_PANEL);

	mainMenuOpenDelay_ = 1.0f;
}

void PregameState::HandleEnter()
{
	if(mainMenu_ != nullptr)
	{
		TaskManager::Add()->Initialize(mainMenu_, &Element::Open, mainMenuOpenDelay_);
	}
}

void PregameState::HandleExit()
{
	if(newWorldMenu_ != nullptr)
	{
		newWorldMenu_->Close();
	}

	if(worldPreviewPanel_ != nullptr)
	{
		worldPreviewPanel_->Close();
	}
}
