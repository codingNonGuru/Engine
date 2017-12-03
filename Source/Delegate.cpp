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

template <class ClassType, class ObjectType>
void Delegate::Add(ObjectType *object, void (ClassType::*function)())
{
	auto callback = callbacks_.allocate();
	if(callback == nullptr)
		return;

	callback->bind(object, function);
}

void Delegate::Add(void (*function)())
{
	auto callback = callbacks_.allocate();
	if(callback == nullptr)
		return;

	callback->bind(function);
}

template <class ClassType, class ObjectType>
void Delegate::Remove(ObjectType *object, void (ClassType::*function)())
{
	Callback externalCallback(object, function);

	for(auto callback = callbacks_.getStart(); callback != callbacks_.getEnd(); ++callback)
	{
		if(*callback == externalCallback)
		{
			callback->clear();
			break;
		}
	}
}
