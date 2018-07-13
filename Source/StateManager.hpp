#pragma once

#include "Types.hpp"
#include "Conventions.hpp"

class State;

class StateManager
{
	static States currentState_;

	static States nextState_;

	static States previousState_;

	static Map <State*, States> states_;

public:
	static void Enter(States);

	static void Register(State*, States);

	static States GetNextState();

	static States GetPreviousState();
};
