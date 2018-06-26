#include "MainMenu.hpp"
#include "Interface/Interface.hpp"
#include "Animator.hpp"
#include "Animation.hpp"
#include "AnimationEvent.hpp"
#include "Delegate.hpp"
#include "Render/Filter.hpp"
#include "Render/FilterManager.hpp"

void MainMenu::HandleInitialize()
{
	saturateFilter_ = FilterManager::GetFilter("Saturate");
	blurFilter_ = FilterManager::GetFilter("Blur");

	newGameMenu_ = Interface::GetElement("NewGameMenu");

	auto animation = animator_->GetAnimation("Close");
	if(!animation)
		return;

	auto event = animation->GetFinishEvent();
	event->GetActions().Add(this, &MainMenu::Disable);
	event->GetActions().Add(this, &MainMenu::OpenNewGameMenu);
}

void MainMenu::HandleOpen()
{
	if(saturateFilter_)
	{
		saturateFilter_->Open();
	}

	if(blurFilter_)
	{
		blurFilter_->Open();
	}
}

void MainMenu::HandleClose()
{
	if(saturateFilter_)
	{
		saturateFilter_->Close();
	}

	if(blurFilter_)
	{
		blurFilter_->Close();
	}
}

void MainMenu::OpenNewGameMenu()
{
	if(newGameMenu_)
	{
		newGameMenu_->Open();
	}
}
