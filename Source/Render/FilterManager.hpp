#pragma once

#include "Conventions.hpp"

class Filter;
class Camera;

class FilterManager
{
private:
	static Map <Filter*, Word> filters_;

public:
	static Map <Filter*, Word> & GetFilters();

	static Filter* GetFilter(const char*);

	static Filter* AddFilter(const char*, Filter*);

	static void Update(Camera*);
};
