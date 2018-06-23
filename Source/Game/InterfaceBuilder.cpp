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

#include "Game/MainMenu.hpp"
#include "Game/NewGameMenu.hpp"
#include "Game/NewWorldMenu.hpp"
#include "Game/CloseButton.hpp"
#include "Game/InterfacePainter.hpp"

Shader* spriteShader = nullptr;
Shader* shadowShader = nullptr;

void InterfaceBuilder::GenerateInterface()
{
	auto glyphShader = ShaderManager::GetShaderMap().Get("Glyph");

	TextManager::Initialize(glyphShader);

	InterfacePainter::PaintInterface();

	spriteShader = ShaderManager::GetShaderMap().Get("Sprite");
	shadowShader = ShaderManager::GetShaderMap().Get("Shadow");

	GenerateMainMenu();
	
	GenerateNewGameMenu();

	GenerateNewWorldMenu();

	auto elements = Interface::GetElements();
	for(auto elementIterator = elements.GetStart(); elementIterator != elements.GetEnd(); ++elementIterator)
	{
		auto element = *elementIterator;
		if(!element)
			continue;

		element->Initialize();
	}
}

void InterfaceBuilder::GenerateMainMenu()
{
	auto texture = TextureManager::GetTextures().Get("MainMenu");
	auto sprite = new Sprite(texture, spriteShader);

	auto mainMenu = Interface::AddElement("MainMenu", new MainMenu());
	mainMenu->Configure(Size(1000, 600), DrawOrder(1), new Transform(Position2(0.0f, 0.0f)), sprite, Opacity(1.0f));

	auto animator = mainMenu->GetAnimator();

	AddOpenAnimation(mainMenu, 1800.0f, 0.0f);

	AddCloseAnimation(mainMenu, -1800.0f, 0.0f);

	texture = TextureManager::GetTextures().Get("MainMenuShadow");
	sprite = new Sprite(texture, shadowShader);

	auto shadow = Interface::AddElement("Shadow", new Element());
	shadow->Configure(Size(150, 150), DrawOrder(0), new Transform(Position2(0.0f, 0.0f)), sprite, Opacity(1.0f));

	shadow->Enable();
	shadow->SetParent(mainMenu);

	GenerateMainMenuNewGameButton();
}

void InterfaceBuilder::GenerateMainMenuNewGameButton()
{
	auto texture = TextureManager::GetTextures().Get("MainMenuCloseButton");
	auto sprite = new Sprite(texture, spriteShader);

	auto button = Interface::AddElement("CloseButton", new CloseButton());
	button->Configure(Size(512, 256), DrawOrder(3), new Transform(Position2(0.0f, -300.0f)), sprite, Opacity(1.0f));

	button->Enable();
	auto mainMenu = Interface::GetElement("MainMenu");
	button->SetParent(mainMenu);

	button->SetInteractivity(true);
	button->GetClickEvents().Add(mainMenu, &Element::Close);

	texture = TextureManager::GetTextures().Get("MainMenuCloseButtonShadow");
	sprite = new Sprite(texture, shadowShader);

	auto shadow = Interface::AddElement("Shadow", new Element());
	shadow->Configure(Size(150, 150), DrawOrder(2), new Transform(Position2(0.0f, 0.0f)), sprite, Opacity(1.0f));

	shadow->Enable();
	shadow->SetParent(button);

	auto font = FontManager::GetFont("Dominican");
	auto text = Interface::AddElement("Text", new Text(font, Color(0.5f, 0.1f, 0.05f)));
	text->Configure(Size(150, 150), DrawOrder(4), new Transform(Position2(0.0f, 0.0f)), nullptr);
	((Text*)text)->Setup("New Game");

	text->Enable();
	text->SetParent(button);
}

void InterfaceBuilder::GenerateNewGameMenu()
{
	auto texture = TextureManager::GetTextures().Get("NewGameMenu");
	auto sprite = new Sprite(texture, spriteShader);

	auto menu = Interface::AddElement("NewGameMenu", new NewGameMenu());
	menu->Configure(Size(400, 400), DrawOrder(-1), new Transform(Position2(0.0f, 0.0f)), sprite);

	auto animator = menu->GetAnimator();

	AddOpenAnimation(menu, 1800.0f, 0.0f);

	AddCloseAnimation(menu, -1800.0f, 0.0f);

	texture = TextureManager::GetTextures().Get("NewGameMenuShadow");
	sprite = new Sprite(texture, shadowShader);

	auto shadow = Interface::AddElement("Shadow", new Element());
	shadow->Configure(Size(400, 400), DrawOrder(-2), new Transform(Position2(0.0f, 0.0f)), sprite, Opacity(1.0f));

	shadow->Enable();
	shadow->SetParent(menu);

	GenerateNewWorldButton();

	GenerateNewGameMenuBackButton();
}

void InterfaceBuilder::GenerateNewWorldButton()
{
	auto texture = TextureManager::GetTextures().Get("NewGameMenuNewWorldButton");
	auto sprite = new Sprite(texture, spriteShader);

	auto button = Interface::AddElement("NewWorldButton", new CloseButton());
	button->Configure(Size(512, 256), DrawOrder(3), new Transform(Position2(0.0f, -300.0f)), sprite, Opacity(1.0f));

	button->Enable();
	auto parent = Interface::GetElement("NewGameMenu");
	button->SetParent(parent);

	button->SetInteractivity(true);
	button->GetClickEvents().Add(parent, &NewGameMenu::OpenNewWorldMenu);

	texture = TextureManager::GetTextures().Get("NewGameMenuNewWorldButtonShadow");
	sprite = new Sprite(texture, shadowShader);

	auto shadow = Interface::AddElement("Shadow", new Element());
	shadow->Configure(Size(150, 150), DrawOrder(2), new Transform(Position2(0.0f, 0.0f)), sprite, Opacity(1.0f));

	shadow->Enable();
	shadow->SetParent(button);

	auto font = FontManager::GetFont("Dominican");
	auto text = Interface::AddElement("Text", new Text(font, Color(0.5f, 0.1f, 0.05f)));
	text->Configure(Size(150, 150), DrawOrder(4), new Transform(Position2(0.0f, 0.0f)), nullptr);
	((Text*)text)->Setup("New World");

	text->Enable();
	text->SetParent(button);
}

void InterfaceBuilder::GenerateNewGameMenuBackButton()
{
	auto texture = TextureManager::GetTextures().Get("NewGameMenuBackButton");
	auto sprite = new Sprite(texture, spriteShader);

	auto button = Interface::AddElement("BackButton", new Element());
	button->Configure(Size(512, 256), DrawOrder(3), new Transform(Position2(0.0f, 300.0f)), sprite, Opacity(1.0f));

	button->Enable();
	auto parent = Interface::GetElement("NewGameMenu");
	button->SetParent(parent);

	button->SetInteractivity(true);
	button->GetClickEvents().Add(parent, &NewGameMenu::OpenMainMenu);

	texture = TextureManager::GetTextures().Get("NewGameMenuBackButtonShadow");
	sprite = new Sprite(texture, shadowShader);

	auto shadow = Interface::AddElement("Shadow", new Element());
	shadow->Configure(Size(150, 150), DrawOrder(2), new Transform(Position2(0.0f, 0.0f)), sprite, Opacity(1.0f));

	shadow->Enable();
	shadow->SetParent(button);

	auto font = FontManager::GetFont("Dominican");
	auto text = Interface::AddElement("Text", new Text(font, Color(0.5f, 0.1f, 0.05f)));
	text->Configure(Size(150, 150), DrawOrder(4), new Transform(Position2(0.0f, 0.0f)), nullptr);
	((Text*)text)->Setup("Back");

	text->Enable();
	text->SetParent(button);
}

void InterfaceBuilder::GenerateNewWorldMenu()
{
	auto texture = TextureManager::GetTextures().Get("NewWorldMenu");
	auto sprite = new Sprite(texture, spriteShader);

	auto menu = Interface::AddElement("NewWorldMenu", new NewWorldMenu());
	menu->Configure(Size(400, 400), DrawOrder(-1), new Transform(Position2(0.0f, 0.0f)), sprite);

	AddOpenAnimation(menu, 1800.0f, 0.0f);

	AddCloseAnimation(menu, -1800.0f, 0.0f);

	texture = TextureManager::GetTextures().Get("NewWorldMenuShadow");
	sprite = new Sprite(texture, shadowShader);

	auto shadow = Interface::AddElement("Shadow", new Element());
	shadow->Configure(Size(400, 400), DrawOrder(-2), new Transform(Position2(0.0f, 0.0f)), sprite, Opacity(1.0f));

	shadow->Enable();
	shadow->SetParent(menu);

	GenerateNewWorldMenuSizeButton();

	GenerateNewWorldMenuLeftScrollButton();

	GenerateNewWorldMenuRightScrollButton();

	GenerateNewWorldMenuGenerateButton();

	GenerateNewWorldMenuBackButton();
}

void InterfaceBuilder::GenerateNewWorldMenuSizeButton()
{
	auto texture = TextureManager::GetTextures().Get("NewWorldMenuSetSizeButton");
	auto sprite = new Sprite(texture, spriteShader);

	auto button = Interface::AddElement("SizeButton", new Element());
	button->Configure(Size(512, 256), DrawOrder(3), new Transform(Position2(0.0f, -300.0f)), sprite, Opacity(1.0f));

	button->Enable();
	auto parent = Interface::GetElement("NewWorldMenu");
	button->SetParent(parent);

	//button->SetInteractivity(true);
	//button->GetClickEvents().Add(parent, )

	auto font = FontManager::GetFont("Dominican");
	auto text = Interface::AddElement("SizeLabel", new Text(font, Color(0.5f, 0.1f, 0.05f)));
	text->Configure(Size(100, 100), DrawOrder(4), new Transform(Position2(0.0f, 0.0f)), nullptr);
	((Text*)text)->Setup("512 x 512");

	text->Enable();
	text->SetParent(button);

	texture = TextureManager::GetTextures().Get("NewWorldMenuSetSizeButtonShadow");
	sprite = new Sprite(texture, shadowShader);

	auto shadow = Interface::AddElement("Shadow", new Element());
	shadow->Configure(Size(150, 150), DrawOrder(2), new Transform(Position2(0.0f, 0.0f)), sprite, Opacity(1.0f));

	shadow->Enable();
	shadow->SetParent(button);
}

void InterfaceBuilder::GenerateNewWorldMenuLeftScrollButton()
{
	auto texture = TextureManager::GetTextures().Get("NewWorldMenuLeftScrollButton");
	auto sprite = new Sprite(texture, spriteShader);

	auto button = Interface::AddElement("LeftScrollButton", new Element());
	button->Configure(Size(256, 256), DrawOrder(3), new Transform(Position2(-340.0f, -300.0f)), sprite, Opacity(1.0f));

	button->Enable();
	auto parent = Interface::GetElement("NewWorldMenu");
	button->SetParent(parent);

	button->SetInteractivity(true);
	button->GetClickEvents().Add(parent, &NewWorldMenu::ScrollLeftwards);

	texture = TextureManager::GetTextures().Get("NewWorldMenuLeftScrollButtonShadow");
	sprite = new Sprite(texture, shadowShader);

	auto shadow = Interface::AddElement("Shadow", new Element());
	shadow->Configure(Size(150, 150), DrawOrder(2), new Transform(Position2(0.0f, 0.0f)), sprite, Opacity(1.0f));

	shadow->Enable();
	shadow->SetParent(button);

	texture = TextureManager::GetTextures().Get("Arrow");
	sprite = new Sprite(texture, spriteShader);

	auto icon = Interface::AddElement("ArrowIcon", new Element());
	icon->Configure(Size(256, 256), DrawOrder(4), new Transform(Position2(0.0f, 0.0f)), sprite, Opacity(1.0f));

	icon->Enable();
	icon->SetParent(button);
}

void InterfaceBuilder::GenerateNewWorldMenuRightScrollButton()
{
	auto texture = TextureManager::GetTextures().Get("NewWorldMenuRightScrollButton");
	auto sprite = new Sprite(texture, spriteShader);

	auto button = Interface::AddElement("RightScrollButton", new Element());
	button->Configure(Size(256, 256), DrawOrder(3), new Transform(Position2(340.0f, -300.0f)), sprite, Opacity(1.0f));

	button->Enable();
	auto parent = Interface::GetElement("NewWorldMenu");
	button->SetParent(parent);

	button->SetInteractivity(true);
	button->GetClickEvents().Add(parent, &NewWorldMenu::ScrollRightwards);

	texture = TextureManager::GetTextures().Get("NewWorldMenuRightScrollButtonShadow");
	sprite = new Sprite(texture, shadowShader);

	auto shadow = Interface::AddElement("Shadow", new Element());
	shadow->Configure(Size(150, 150), DrawOrder(2), new Transform(Position2(0.0f, 0.0f)), sprite, Opacity(1.0f));

	shadow->Enable();
	shadow->SetParent(button);

	texture = TextureManager::GetTextures().Get("RightArrow");
	sprite = new Sprite(texture, spriteShader);

	auto icon = Interface::AddElement("ArrowIcon", new Element());
	icon->Configure(Size(256, 256), DrawOrder(4), new Transform(Position2(0.0f, 0.0f)), sprite, Opacity(1.0f));

	icon->Enable();
	icon->SetParent(button);
}

void InterfaceBuilder::GenerateNewWorldMenuGenerateButton()
{
	auto texture = TextureManager::GetTextures().Get("NewWorldMenuGenerateButton");
	auto sprite = new Sprite(texture, spriteShader);

	auto button = Interface::AddElement("GenerateButton", new Element());
	button->Configure(Size(256, 256), DrawOrder(3), new Transform(Position2(0.0f, -100.0f)), sprite, Opacity(1.0f));

	button->Enable();
	auto parent = Interface::GetElement("NewWorldMenu");
	button->SetParent(parent);

	button->SetInteractivity(true);
	button->GetClickEvents().Add(parent, &NewWorldMenu::GenerateWorld);

	texture = TextureManager::GetTextures().Get("NewWorldMenuGenerateButtonShadow");
	sprite = new Sprite(texture, shadowShader);

	auto shadow = Interface::AddElement("Shadow", new Element());
	shadow->Configure(Size(150, 150), DrawOrder(2), new Transform(Position2(0.0f, 0.0f)), sprite, Opacity(1.0f));

	shadow->Enable();
	shadow->SetParent(button);

	auto font = FontManager::GetFont("Dominican");
	auto text = Interface::AddElement("Text", new Text(font, Color(0.5f, 0.1f, 0.05f)));
	text->Configure(Size(100, 100), DrawOrder(4), new Transform(Position2(0.0f, 0.0f)), nullptr);
	((Text*)text)->Setup("Generate");

	text->Enable();
	text->SetParent(button);
}

void InterfaceBuilder::GenerateNewWorldMenuBackButton()
{
	auto texture = TextureManager::GetTextures().Get("NewWorldMenuBackButton");
	auto sprite = new Sprite(texture, spriteShader);

	auto button = Interface::AddElement("BackButton", new Element());
	button->Configure(Size(256, 256), DrawOrder(3), new Transform(Position2(0.0f, 300.0f)), sprite, Opacity(1.0f));

	button->Enable();
	auto parent = Interface::GetElement("NewWorldMenu");
	button->SetParent(parent);

	button->SetInteractivity(true);
	button->GetClickEvents().Add(parent, &NewWorldMenu::OpenNewGameMenu);

	texture = TextureManager::GetTextures().Get("NewWorldMenuBackButtonShadow");
	sprite = new Sprite(texture, shadowShader);

	auto shadow = Interface::AddElement("Shadow", new Element());
	shadow->Configure(Size(150, 150), DrawOrder(2), new Transform(Position2(0.0f, 0.0f)), sprite, Opacity(1.0f));

	shadow->Enable();
	shadow->SetParent(button);

	auto font = FontManager::GetFont("Dominican");
	auto text = Interface::AddElement("Text", new Text(font, Color(0.5f, 0.1f, 0.05f)));
	text->Configure(Size(100, 100), DrawOrder(4), new Transform(Position2(0.0f, 0.0f)), nullptr);
	((Text*)text)->Setup("Back");

	text->Enable();
	text->SetParent(button);
}

void InterfaceBuilder::AddOpenAnimation(Element* element, float startHeight, float endHeight)
{
	auto animator = element->GetAnimator();
	if(animator == nullptr)
		return;

	auto animation = animator->AddAnimation(new Animation(0.5f), "Open");
	{
		auto property = element->AddAnimationProperty("Open", InterfaceElementParameters::POSITION_X);
		property->AddKey()->Initialize(0.0f, startHeight);
		property->AddKey()->Initialize(animation->GetLength(), endHeight);
	}
	{
		auto property = element->AddAnimationProperty("Open", InterfaceElementParameters::OPACITY);
		property->AddKey()->Initialize(0.0f, 0.0f);
		property->AddKey()->Initialize(animation->GetLength(), 1.0f);
	}
}

void InterfaceBuilder::AddCloseAnimation(Element* element, float startHeight, float endHeight)
{
	auto animator = element->GetAnimator();
	if(animator == nullptr)
		return;

	auto animation = animator->AddAnimation(new Animation(0.5f), "Close");
	{
		auto property = element->AddAnimationProperty("Close", InterfaceElementParameters::POSITION_X);
		property->AddKey()->Initialize(0.0f, endHeight);
		property->AddKey()->Initialize(animation->GetLength(), startHeight);
	}
	{
		auto property = element->AddAnimationProperty("Close", InterfaceElementParameters::OPACITY);
		property->AddKey()->Initialize(0.0f, 1.0f);
		property->AddKey()->Initialize(animation->GetLength(), 0.0f);
	}
}

