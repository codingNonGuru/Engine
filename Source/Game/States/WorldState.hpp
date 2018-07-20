#pragma once

#include "State.hpp"

class Scene;

class WorldState : public State
{
private:
	Scene* worldScene_;

	void HandleSceneEnable();

protected:
	virtual void HandleEnter() override;

	virtual void HandleExit() override;

public:
	WorldState();
};
