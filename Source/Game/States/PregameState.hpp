#pragma once

#include "State.hpp"

class Element;

class PregameState : public State
{
private:
	Element* mainMenu_;

	Element* newWorldMenu_;

	Element* worldPreviewPanel_;

	float mainMenuOpenDelay_;

protected:
	virtual void HandleEnter() override;

	virtual void HandleExit() override;

public:
	PregameState();
};
