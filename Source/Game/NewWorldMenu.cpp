#include "Animator.hpp"
#include "Animation.hpp"
#include "AnimationEvent.hpp"
#include "Delegate.hpp"
#include "Interface/Interface.hpp"
#include "Interface/Text.hpp"
#include "SceneManager.hpp"

#include "NewWorldMenu.hpp"
#include "Game/MainScene.hpp"
#include "Game/WorldParameterSet.hpp"

void NewWorldMenu::HandleInitialize()
{
	newGameMenu_ = Interface::GetElement("NewGameMenu");

	leftScrollButton_ = GetChild("LeftScrollButton");

	rightScrollButton_ = GetChild("RightScrollButton");

	auto sizeButton = GetChild("SizeButton");
	if(sizeButton != nullptr)
	{
		sizeLabel_ = sizeButton->GetChild("SizeLabel");
	}

	mainScene_ = SceneManager::GetScene("MainScene");

	auto animation = animator_->GetAnimation("Close");
	if(animation == nullptr)
		return;

	closeEvent_ = animation->AddEvent();
	if(closeEvent_ == nullptr)
		return;

	closeEvent_->Initialize(0.5f);
	closeEvent_->GetEvents().Add(this, &NewWorldMenu::Disable);

	sizeOption_ = SizeOptions::TINY;

	if(leftScrollButton_ != nullptr)
	{
		leftScrollButton_->Disable();
	}

	UpdateSizeLabel();
}

void NewWorldMenu::OpenNewGameMenu()
{
	closeEvent_->GetEvents().Add(this, &NewWorldMenu::HandleNewGameMenuOpen);

	Close();
}

void NewWorldMenu::HandleNewGameMenuOpen()
{
    if(newGameMenu_ != nullptr)
    {
    	newGameMenu_->Open();
    }

    closeEvent_->GetEvents().Remove(this, &NewWorldMenu::HandleNewGameMenuOpen);
}

void NewWorldMenu::ScrollLeftwards()
{
	if(sizeOption_ == SizeOptions::SMALL)
	{
		leftScrollButton_->Disable();
	}

	if(sizeOption_ == SizeOptions::IMMENSE)
	{
		rightScrollButton_->Enable();
	}

	sizeOption_ = SizeOptions((int)sizeOption_ - 1);

	UpdateSizeLabel();
}

void NewWorldMenu::ScrollRightwards()
{
	if(sizeOption_ == SizeOptions::LARGE)
	{
		rightScrollButton_->Disable();
	}

	if(sizeOption_ == SizeOptions::TINY)
	{
		leftScrollButton_->Enable();
	}

	sizeOption_ = SizeOptions((int)sizeOption_ + 1);

	UpdateSizeLabel();
}

void NewWorldMenu::UpdateSizeLabel()
{
	if(sizeLabel_ == nullptr)
		return;

	switch(sizeOption_)
	{
	case SizeOptions::TINY:
		sizeLabel_->Setup("256 x 256");
		break;
	case SizeOptions::SMALL:
		sizeLabel_->Setup("512 x 512");
		break;
	case SizeOptions::MEDIUM:
		sizeLabel_->Setup("768 x 768");
		break;
	case SizeOptions::LARGE:
		sizeLabel_->Setup("1024 x 1024");
		break;
	case SizeOptions::IMMENSE:
		sizeLabel_->Setup("1536 x 1536");
		break;
	}
}

void NewWorldMenu::GenerateWorld()
{
	auto generateButton = GetChild("GenerateButton");
	if(generateButton != nullptr)
	{
		generateButton->Disable();
	}

	if(leftScrollButton_ != nullptr)
	{
		leftScrollButton_->Disable();
	}

	if(rightScrollButton_ != nullptr)
	{
		rightScrollButton_->Disable();
	}

	auto backButton = GetChild("BackButton");
	if(backButton != nullptr)
	{
		backButton->Disable();
	}

	if(mainScene_ != nullptr)
	{
		WorldParameterSet parameterSet;
		switch(sizeOption_)
		{
		case SizeOptions::TINY:
			parameterSet.Size_ = Size(256, 256);
			break;
		case SizeOptions::SMALL:
			parameterSet.Size_ = Size(512, 512);
			break;
		case SizeOptions::MEDIUM:
			parameterSet.Size_ = Size(768, 768);
			break;
		case SizeOptions::LARGE:
			parameterSet.Size_ = Size(1024, 1024);
			break;
		case SizeOptions::IMMENSE:
			parameterSet.Size_ = Size(1536, 1536);
			break;
		}

		mainScene_->Initialize(parameterSet);
	}
}

