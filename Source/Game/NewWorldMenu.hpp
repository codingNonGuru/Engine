#pragma once

#include "Interface/Element.hpp"

class AnimationEvent;
class Text;
class MainScene;

class NewWorldMenu : public Element
{
	enum class SizeOptions {TINY, SMALL, MEDIUM, LARGE, IMMENSE};

	Element* newGameMenu_;

	Element* leftScrollButton_;

	Element* rightScrollButton_;

	Text* sizeLabel_;

	AnimationEvent* closeEvent_;

	MainScene* mainScene_;

	SizeOptions sizeOption_;

	void HandleNewGameMenuOpen();

	void UpdateSizeLabel();

public:
	virtual void HandleInitialize() override;

	void OpenNewGameMenu();

	void ScrollLeftwards();

	void ScrollRightwards();

	void GenerateWorld();
};
