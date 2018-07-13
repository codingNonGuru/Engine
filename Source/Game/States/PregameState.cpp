#include "Interface/Interface.hpp"
#include "Interface/Element.hpp"

#include "Game/States/PregameState.hpp"

PregameState::PregameState()
{
	mainMenu_ = Interface::GetElement("MainMenu");
}

void PregameState::HandleEnter()
{
	if(mainMenu_ != nullptr)
	{
		mainMenu_->Open();
	}
}

void PregameState::HandleExit()
{
}
