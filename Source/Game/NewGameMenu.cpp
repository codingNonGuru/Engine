#include "Animator.hpp"
#include "Animation.hpp"
#include "AnimationEvent.hpp"
#include "Delegate.hpp"
#include "Interface/Interface.hpp"

#include "Game/NewGameMenu.hpp"
#include "Game/Types.hpp"

void NewGameMenu::HandleInitialize()
{
	mainMenu_ = Interface::GetElement("MainMenu");

	newWorldMenu_ = Interface::GetElement(Elements::NEW_WORLD_MENU);
}

void NewGameMenu::OpenNewWorldMenu()
{
	closeEvent_->GetActions().Add(this, &NewGameMenu::HandleNewWorldOpen);

	Close();
}

void NewGameMenu::HandleNewWorldOpen()
{
    if(newWorldMenu_ != nullptr)
    {
    	newWorldMenu_->Open();
    }

    closeEvent_->GetActions().Remove(this, &NewGameMenu::HandleNewWorldOpen);
}

void NewGameMenu::OpenMainMenu()
{
	closeEvent_->GetActions().Add(this, &NewGameMenu::HandleMainMenuOpen);

	Close();
}

void NewGameMenu::HandleMainMenuOpen()
{
    if(mainMenu_ != nullptr)
    {
    	mainMenu_->Open();
    }

    closeEvent_->GetActions().Remove(this, &NewGameMenu::HandleMainMenuOpen);
}
