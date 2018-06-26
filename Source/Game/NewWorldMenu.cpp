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
#include "Game/WorldGenerator.hpp"

void NewWorldMenu::HandleInitialize()
{
	newGameMenu_ = Interface::GetElement("NewGameMenu");

	leftScrollButton_ = GetChild("LeftScrollButton");

	rightScrollButton_ = GetChild("RightScrollButton");

	worldPreview_ = Interface::GetElement("WorldPreview");

	auto sizeButton = GetChild("SizeButton");
	if(sizeButton != nullptr)
	{
		sizeLabel_ = sizeButton->GetChild("SizeLabel");
	}

	mainScene_ = SceneManager::GetScene("MainScene");

	auto animation = animator_->GetAnimation("Close");
	if(animation == nullptr)
		return;

	closeEvent_ = animation->GetFinishEvent();
	closeEvent_->GetActions().Add(this, &NewWorldMenu::Disable);

	sizeOption_ = WorldSizeOptions::SMALL;

	if(sizeOption_ == WorldSizeOptions::TINY && leftScrollButton_ != nullptr)
	{
		leftScrollButton_->Disable();
	}

	UpdateSizeLabel();

	WorldGenerator::OnWorldGenerated_.Add(this, &NewWorldMenu::FinishGeneration);
}

void NewWorldMenu::OpenNewGameMenu()
{
	closeEvent_->GetActions().Add(this, &NewWorldMenu::HandleNewGameMenuOpen);

	Close();
}

void NewWorldMenu::HandleNewGameMenuOpen()
{
    if(newGameMenu_ != nullptr)
    {
    	newGameMenu_->Open();
    }

    closeEvent_->GetActions().Remove(this, &NewWorldMenu::HandleNewGameMenuOpen);
}

void NewWorldMenu::ScrollLeftwards()
{
	if(sizeOption_ == WorldSizeOptions::SMALL)
	{
		leftScrollButton_->Disable();
	}

	if(sizeOption_ == WorldSizeOptions::IMMENSE)
	{
		rightScrollButton_->Enable();
	}

	sizeOption_ = WorldSizeOptions((int)sizeOption_ - 1);

	UpdateSizeLabel();
}

void NewWorldMenu::ScrollRightwards()
{
	if(sizeOption_ == WorldSizeOptions::LARGE)
	{
		rightScrollButton_->Disable();
	}

	if(sizeOption_ == WorldSizeOptions::TINY)
	{
		leftScrollButton_->Enable();
	}

	sizeOption_ = WorldSizeOptions((int)sizeOption_ + 1);

	UpdateSizeLabel();
}

void NewWorldMenu::UpdateSizeLabel()
{
	if(sizeLabel_ == nullptr)
		return;

	switch(sizeOption_)
	{
	case WorldSizeOptions::TINY:
		sizeLabel_->Setup("256 x 256");
		break;
	case WorldSizeOptions::SMALL:
		sizeLabel_->Setup("512 x 512");
		break;
	case WorldSizeOptions::MEDIUM:
		sizeLabel_->Setup("768 x 768");
		break;
	case WorldSizeOptions::LARGE:
		sizeLabel_->Setup("1024 x 1024");
		break;
	case WorldSizeOptions::IMMENSE:
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

	if(worldPreview_ != nullptr)
	{
		worldPreview_->Open();
	}

	auto backButton = GetChild("BackButton");
	if(backButton != nullptr)
	{
		backButton->Disable();
	}

	if(mainScene_ != nullptr)
	{
		WorldParameterSet parameterSet;
		parameterSet.SizeOption_ = sizeOption_;

		mainScene_->Initialize(parameterSet);
	}
}

void NewWorldMenu::FinishGeneration()
{
	auto backButton = GetChild("BackButton");
	if(backButton != nullptr)
	{
		backButton->Enable();
	}
}

