#include "Delegate.hpp"

Delegate::Delegate()
{
	callbacks_.initialize(4);
}

void Delegate::Invoke()
{
	for(auto callback = callbacks_.getStart(); callback != callbacks_.getEnd(); ++callback)
	{
		if(callback->empty())
			continue;

		(*callback)();
	}
}

void Delegate::Add(void (*function)())
{
	auto callback = callbacks_.allocate();
	if(callback == nullptr)
		return;

	callback->bind(function);
}





