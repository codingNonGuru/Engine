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

	void Add(void (*)());

	template <class ClassType, class ObjectType>
	void Remove(ObjectType*, void (ClassType::*)());
};
