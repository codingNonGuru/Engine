#pragma once

#include "Conventions.hpp"

class DataBuffer;

class HeaderBuffer
{
	Index key_;

	container::StaticMap <DataBuffer*> slaveBuffers_;

public:
	HeaderBuffer() {}

	HeaderBuffer(int);

	void Generate();

	void AddBuffer(int, DataBuffer*, const char*);

	void AddBuffer(int, DataBuffer*, const char*, Index, bool);

	void AddElementBuffer(DataBuffer*);

	void AddStorageBuffer(int, DataBuffer*, const char*);

	DataBuffer* GetBuffer(int);

	DataBuffer* GetBuffer(const char*);

	void SetSlaveBindPoint(const char*, Index);

	void UploadData(const char*, void*, Index);

	void Bind();

	void Unbind();
};
