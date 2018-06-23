#pragma once

#include "Interface/Element.hpp"

class Element;
class AnimationEvent;

class NewGameMenu : public Element
{
	Element* mainMenu_;

	Element* newWorldMenu_;

	AnimationEvent* closeEvent_;

	virtual void HandleInitialize();

	void HandleNewWorldOpen();

	void HandleMainMenuOpen();

public:
	void OpenMainMenu();

    void OpenNewWorldMenu();
};
