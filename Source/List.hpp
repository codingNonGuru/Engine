#pragma once

#include "Conventions.hpp"

template <class ObjectType>
class StaticList
{
	struct Node
	{
		ObjectType Object_;

		Node* Previous_;

		Node* Next_;

		Node() : Previous_(nullptr), Next_(nullptr) {}
	};

	Array <ObjectType> objects_;

	Node* lastNode_;

	StaticList()
	{
		lastNode_ = nullptr;
	}

	StaticList(int capacity)
	{
		Initialize(capacity);
	}

	void Initialize(int capacity)
	{
		objects_.Initialize(capacity);

		lastNode_ = nullptr;
	}

	Node* Add()
	{
		auto node = objects_.Allocate();
		if(!node)
			return nullptr;

		if(lastNode_)
		{
			lastNode_->Next_ = node;
		}
		node->Previous_ = lastNode_;

		lastNode_ = node;

		return node;
	}

	Node* GetFirstNode()
	{
		return objects_.GetStart();
	}
};
