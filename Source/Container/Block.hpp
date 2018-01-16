#pragma once

namespace container
{
	template <class ObjectType, int Capacity>
	class Block
	{
		ObjectType objects_[Capacity];

		Block() {}

		ObjectType* Find(int index)
		{
			return objects_ + index;
		}

		ObjectType* operator [] (int index)
		{
			return objects_ + index;
		}

		ObjectType* GetStart()
		{
			return objects_;
		}

		ObjectType* GetEnd()
		{
			return objects_ + Capacity;
		}
	};
}
