#include "Render/FilterManager.hpp"

#include "Render/Filter.hpp"

#define MAXIMUM_FILTER_COUNT 32

Map <Filter*, LongWord> FilterManager::filters_ = Map <Filter*, LongWord> (MAXIMUM_FILTER_COUNT);

Map <Filter*, LongWord> & FilterManager::GetFilters()
{
	return filters_;
}

Filter* FilterManager::GetFilter(const char* name)
{
	return *filters_.Get(name);
}

Filter* FilterManager::AddFilter(const char* name, Filter* filter)
{
	auto filterPointer = filters_.Add(name);
	if(!filterPointer)
		return nullptr;

	*filterPointer = filter;

	return filter;
}
