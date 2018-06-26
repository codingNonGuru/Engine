#pragma once

#include "Interface/Element.hpp"
#include "Game/Types.hpp"

class AnimationEvent;
class Text;
class MainScene;

class NewWorldMenu : public Element
{
	Element* newGameMenu_;

	Element* leftScrollButton_;

	Element* rightScrollButton_;

	Text* sizeLabel_;

	Element* worldPreview_;

	AnimationEvent* closeEvent_;

	MainScene* mainScene_;

	WorldSizeOptions sizeOption_;

	void HandleNewGameMenuOpen();

	void UpdateSizeLabel();

public:
	virtual void HandleInitialize() override;

	void OpenNewGameMenu();

	void ScrollLeftwards();

	void ScrollRightwards();

	void GenerateWorld();

	void FinishGeneration();
};
