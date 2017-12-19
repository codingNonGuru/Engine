#pragma once

#include "Conventions.hpp"

class HeaderBuffer;
class DataBuffer;
class FrameBuffer;

class BufferManager
{
private:
	static Map <HeaderBuffer*, LongWord> headerBuffers_;

	static Map <DataBuffer*, LongWord> dataBuffers_;

	static Map <FrameBuffer*, LongWord> frameBuffers_;

public:
	static Map <HeaderBuffer*, LongWord> & GetHeaderBuffers();

	static Map <DataBuffer*, LongWord> & GetDataBuffers();

	static Map <FrameBuffer*, LongWord> & GetFrameBuffers();

	static HeaderBuffer* GetHeaderBuffer(const char*);

	static DataBuffer* GetDataBuffer(const char*);

	static FrameBuffer* GetFrameBuffer(const char*);
};
