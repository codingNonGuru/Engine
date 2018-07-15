#include "Interface/Interface.hpp"
#include "Interface/Element.hpp"
#include "TaskManager.hpp"

#include "Game/States/PregameState.hpp"

PregameState::PregameState()
{
	mainMenu_ = Interface::GetElement("MainMenu");

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
}
