#include "InterfaceBuilder.hpp"

#include "Interface.hpp"
#include "ShaderManager.hpp"
#include "Sprite.hpp"
#include "Element.hpp"
#include "Shader.hpp"
#include "Animation.hpp"
#include "Animator.hpp"
#include "AnimationProperty.hpp"
#include "AnimationKey.hpp"
#include "AnimationEvent.hpp"
#include "Transform.hpp"
#include "Delegate.hpp"

void InterfaceBuilder::GenerateInterface()
{
	auto spriteShader = ShaderManager::GetShaderMap().Get("Sprite");

	{
		auto sprite = new Sprite(nullptr, spriteShader);

		Element* mainMenu = Interface::GetElements().Allocate(LongWord("MainMenu"));
		mainMenu->Initialize(Size(1000, 600), DrawOrder(0), new Transform(Position2(0.0f, 0.0f)), sprite);

		auto animator = mainMenu->GetAnimator();

		auto animation = animator->AddAnimation(new Animation(0.4f), "Open");
		{
			auto property = mainMenu->AddAnimationProperty("Open", InterfaceElementParameters::POSITION_Y);
			property->AddKey()->Initialize(0.0f, 1200.0f);
			property->AddKey()->Initialize(animation->GetLength(), 0.0f);
		}
		{
			auto property = mainMenu->AddAnimationProperty("Open", InterfaceElementParameters::OPACITY);
			property->AddKey()->Initialize(0.0f, 0.0f);
			property->AddKey()->Initialize(animation->GetLength(), 0.5f);
		}

		animation = animator->AddAnimation(new Animation(0.4f), "Close");
		{
			auto property = mainMenu->AddAnimationProperty("Close", InterfaceElementParameters::POSITION_Y);
			property->AddKey()->Initialize(0.0f, 0.0f);
			property->AddKey()->Initialize(animation->GetLength(), 1200.0f);
		}
		{
			auto property = mainMenu->AddAnimationProperty("Close", InterfaceElementParameters::OPACITY);
			property->AddKey()->Initialize(0.0f, 0.5f);
			property->AddKey()->Initialize(animation->GetLength(), 0.0f);
		}

		auto event = animation->AddEvent();
		event->Initialize(0.4f);
		event->GetEvents().Add(mainMenu, &Element::Disable);

		sprite = new Sprite(nullptr, spriteShader);

		Element* mainMenuCloseButton = Interface::GetElements().Allocate(LongWord("MainMenuCloseButton"));
		mainMenuCloseButton->Initialize(Size(100, 100), DrawOrder(1), new Transform(Position2(500.0f, -300.0f)), sprite, Opacity(0.5f));

		mainMenuCloseButton->Enable();
		mainMenuCloseButton->SetParent(mainMenu);

		mainMenuCloseButton->GetClickEvents().Add(mainMenu, &Element::Close);
	}
}
