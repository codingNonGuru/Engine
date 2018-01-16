#pragma once

#include "Container/Container.hpp"

namespace container
{
	template<class O, typename I = int>
	class Array : public Container
	{
		O* objects_;

		I capacity_;

		I size_;

		unsigned long memorySize_;

		virtual void* GetData() {return (void*)objects_;}

	public:
		Array() : objects_(nullptr), size_(0), capacity_(0) {}

		Array(I capacity) : capacity_(capacity), size_(0)
		{
			memorySize_ = capacity_ * sizeof(O);
			objects_ = new O[capacity_];

			MemoryLog::accrue(memorySize_);
		}

		void Initialize(I capacity) {
			capacity_ = capacity;
			memorySize_ = capacity_ * sizeof(O);
			size_ = 0;
			Destroy();
			objects_ = new O[capacity_];

			MemoryLog::accrue(memorySize_);
		}

		void Reset()
		{
			size_ = 0;
		}

		bool IsFull()
		{
			return size_ == capacity_;
		}

		O *const Allocate(I count)
		{
			O *const newObject = (objects_ + size_);
			size_ += count;
			return newObject;
		}

		O *const Allocate()
		{
			if(size_ == capacity_)
				return nullptr;

			O *const newObject = (objects_ + size_);
			size_++;

			return newObject;
		}

		template<class DerivedType>
		O *const Allocate()
		{
			if(size_ == capacity_)
				return nullptr;

			O *const newObject = (objects_ + size_);
			*(long*)newObject = *(long*)&DerivedType();
			size_++;

			return newObject;
		}

		O& operator[] (I index)
		{
			if(index < 0)
				index += capacity_;
			if(index >= capacity_)
				index -= capacity_;

			return *(objects_ + index);
		}

		O* Get(I index)
		{
			if(index < 0)
				index += capacity_;
			if(index >= capacity_)
				index -= capacity_;

			return objects_ + index;
		}

		O* GetStart()
		{
			return objects_;
		}

		O* GetEnd()
		{
			return (objects_ + size_);
		}

		O* GetNext(O* object)
		{
			auto index = object - objects_;
			if(index >= size_ - 1)
				return nullptr;

			return object + 1;
		}

		I GetSize() const
		{
			return size_;
		}

		I GetCapacity() const
		{
			return capacity_;
		}

		unsigned long GetMemorySize()
		{
			return size_ * sizeof(O);
		}

		virtual unsigned long GetMemoryCapacity()
		{
			return capacity_ * sizeof(O);
		}

		void Destroy()
		{
			if(objects_ != nullptr)
				delete[] objects_;

			objects_ = nullptr;

			//MemoryLog::accrue(-memorySize_);
		}
	};
}
