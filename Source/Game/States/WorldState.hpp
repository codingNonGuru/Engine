#pragma once

#include "State.hpp"

class Scene;
class Element;

class WorldState : public State
{
private:
	Scene* worldScene_;

	Element* bottomInfoPanel_;

	void HandleSceneEnable();

protected:
	virtual void HandleEnter() override;

	virtual void HandleExit() override;

public:
	WorldState();
};
