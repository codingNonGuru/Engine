#pragma once

#include "Conventions.hpp"

class Filter;

class FilterManager
{
private:
	static Map <Filter*, LongWord> filters_;

public:
	static Map <Filter*, LongWord> & GetFilters();

	static Filter* GetFilter(const char*);

	static Filter* AddFilter(const char*, Filter*);
};
