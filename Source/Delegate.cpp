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
