#include "State.hpp"
#include "StateManager.hpp"

State::State() {}

State::State(States stateType)
{
	StateManager::Register(this, stateType);
	std::cout<<"PELEEEEEEEEEEEEEEEEU\n";
}

void State::Enter()
{
	HandleEnter();
}

void State::Exit()
{
	HandleExit();
}

void State::HandleEnter() {}

void State::HandleExit() {}
