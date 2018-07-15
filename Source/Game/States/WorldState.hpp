#pragma once

#include "State.hpp"

class WorldState : public State
{
protected:
	virtual void HandleEnter() override;

	virtual void HandleExit() override;
};
