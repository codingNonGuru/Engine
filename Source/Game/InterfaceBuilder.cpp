#include "InterfaceBuilder.hpp"

#include "Interface/Interface.hpp"
#include "ShaderManager.hpp"
#include "Interface/Sprite.hpp"
#include "Interface/Element.hpp"
#include "Shader.hpp"
#include "Animation.hpp"
#include "Animator.hpp"
#include "AnimationProperty.hpp"
#include "AnimationKey.hpp"
#include "AnimationEvent.hpp"
#include "Transform.hpp"
#include "Delegate.hpp"
#include "TextureManager.hpp"
#include "Texture.hpp"
#include "Interface/Font.hpp"
#include "Interface/FontManager.hpp"
#include "Interface/TextManager.hpp"
#include "Interface/Text.hpp"

#include "Game/CloseButton.hpp"
#include "Game/InterfacePainter.hpp"

void InterfaceBuilder::GenerateInterface()
{
	auto glyphShader = ShaderManager::GetShaderMap().Get("Glyph");

	TextManager::Initialize(glyphShader);

	InterfacePainter::PaintInterface();

	auto spriteShader = ShaderManager::GetShaderMap().Get("Sprite");
	auto shadowShader = ShaderManager::GetShaderMap().Get("Shadow");

	{
		auto texture = TextureManager::GetTextures().Get("MainMenu");
		auto sprite = new Sprite(texture, spriteShader);

		auto mainMenuPointer = Interface::GetElements().Add("MainMenu");
		*mainMenuPointer = new Element(Size(1000, 600), DrawOrder(1), new Transform(Position2(0.0f, 0.0f)), sprite);
		Element* mainMenu = *mainMenuPointer;

		auto animator = mainMenu->GetAnimator();

		auto animation = animator->AddAnimation(new Animation(0.5f), "Open");
		{
			auto property = mainMenu->AddAnimationProperty("Open", InterfaceElementParameters::POSITION_Y);
			property->AddKey()->Initialize(0.0f, -1200.0f);
			property->AddKey()->Initialize(animation->GetLength(), 0.0f);
		}
		{
			auto property = mainMenu->AddAnimationProperty("Open", InterfaceElementParameters::OPACITY);
			property->AddKey()->Initialize(0.0f, 0.0f);
			property->AddKey()->Initialize(animation->GetLength(), 1.0f);
		}

		animation = animator->AddAnimation(new Animation(0.5f), "Close");
		{
			auto property = mainMenu->AddAnimationProperty("Close", InterfaceElementParameters::POSITION_Y);
			property->AddKey()->Initialize(0.0f, 0.0f);
			property->AddKey()->Initialize(animation->GetLength(), -1200.0f);
		}
		{
			auto property = mainMenu->AddAnimationProperty("Close", InterfaceElementParameters::OPACITY);
			property->AddKey()->Initialize(0.0f, 1.0f);
			property->AddKey()->Initialize(animation->GetLength(), 0.0f);
		}

		auto event = animation->AddEvent();
		event->Initialize(0.5f);
		event->GetEvents().Add(mainMenu, &Element::Disable);

		texture = TextureManager::GetTextures().Get("MainMenuCloseButton");
		sprite = new Sprite(texture, spriteShader);

		auto mainMenuCloseButton = Interface::AddElement("MainMenuCloseButton", new CloseButton());
		mainMenuCloseButton->Initialize(Size(150, 150), DrawOrder(3), new Transform(Position2(0.0f, -250.0f)), sprite, Opacity(1.0f));

		mainMenuCloseButton->Enable();
		mainMenuCloseButton->SetParent(mainMenu);

		mainMenuCloseButton->SetInteractivity(true);
		mainMenuCloseButton->GetClickEvents().Add(mainMenu, &Element::Close);

		texture = TextureManager::GetTextures().Get("MainMenuShadow");
		sprite = new Sprite(texture, shadowShader);

		auto mainMenuShadow = Interface::AddElement("MainMenuShadow", new Element());
		mainMenuShadow->Initialize(Size(150, 150), DrawOrder(0), new Transform(Position2(0.0f, 0.0f)), sprite, Opacity(1.0f));

		mainMenuShadow->Enable();
		mainMenuShadow->SetParent(mainMenu);

		texture = TextureManager::GetTextures().Get("MainMenuCloseButtonShadow");
		sprite = new Sprite(texture, shadowShader);

		auto mainMenuCloseButtonShadow = Interface::AddElement("MainMenuCloseButtonShadow", new Element());
		mainMenuCloseButtonShadow->Initialize(Size(150, 150), DrawOrder(2), new Transform(Position2(0.0f, 0.0f)), sprite, Opacity(1.0f));

		mainMenuCloseButtonShadow->Enable();
		mainMenuCloseButtonShadow->SetParent(mainMenuCloseButton);

		auto font = FontManager::GetFont("Dominican");
		auto text = Interface::AddElement("Text", new Text(font, Color(0.5f, 0.1f, 0.05f)));
		text->Initialize(Size(150, 150), DrawOrder(4), new Transform(Position2(0.0f, 0.0f)), nullptr);
		((Text*)text)->Setup("New Game");

		text->Enable();
		text->SetParent(mainMenuCloseButton);
	}
}
