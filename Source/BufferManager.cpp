#include "BufferManager.hpp"

#include "HeaderBuffer.hpp"
#include "DataBuffer.hpp"
#include "FrameBuffer.hpp"

#define MAXIMUM_HEADER_BUFFER_COUNT 128

#define MAXIMUM_DATA_BUFFER_COUNT 1024

#define MAXIMUM_FRAME_BUFFER_COUNT 32

Map <HeaderBuffer*, LongWord> BufferManager::headerBuffers_ = Map <HeaderBuffer*, LongWord>(MAXIMUM_HEADER_BUFFER_COUNT);

Map <DataBuffer*, LongWord> BufferManager::dataBuffers_ = Map <DataBuffer*, LongWord>(MAXIMUM_DATA_BUFFER_COUNT);

Map <FrameBuffer*, LongWord> BufferManager::frameBuffers_ = Map <FrameBuffer*, LongWord>(MAXIMUM_FRAME_BUFFER_COUNT);

Map <HeaderBuffer*, LongWord> & BufferManager::GetHeaderBuffers()
{
	return headerBuffers_;
}

Map <DataBuffer*, LongWord> & BufferManager::GetDataBuffers()
{
	return dataBuffers_;
}

Map <FrameBuffer*, LongWord> & BufferManager::GetFrameBuffers()
{
	return frameBuffers_;
}

HeaderBuffer* BufferManager::GetHeaderBuffer(const char* name)
{
	auto buffer = headerBuffers_.Get(LongWord(name));
	if(buffer == nullptr)
		return nullptr;

	return *buffer;
}

DataBuffer* BufferManager::GetDataBuffer(const char* name)
{
	auto buffer = dataBuffers_.Get(LongWord(name));
	if(buffer == nullptr)
		return nullptr;

	return *buffer;
}

FrameBuffer* BufferManager::GetFrameBuffer(const char* name)
{
	auto buffer = frameBuffers_.Get(LongWord(name));
	if(buffer == nullptr)
		return nullptr;

	return *buffer;
}
