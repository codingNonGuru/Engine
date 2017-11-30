#pragma once

#include "Conventions.hpp"

class Delegate
{
	Pool <Callback> callbacks_;
public:
	Delegate();

	void Invoke();

	template <class ClassType, class ObjectType>
	void Add(ObjectType*, void (ClassType::*)());

	template <class ClassType, class ObjectType>
	void Remove(ObjectType*, void (ClassType::*)());
};

template <class ClassType, class ObjectType>
void Delegate::Add(ObjectType *object, void (ClassType::*function)())
{
	auto callback = callbacks_.allocate();
	if(callback == nullptr)
		return;

	callback->bind(object, function);
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
		}
	}
}
