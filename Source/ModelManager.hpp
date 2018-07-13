#pragma once

#include "Conventions.hpp"

class Model;

class ModelManager
{
private:
	static Map <Model, LongWord> models_;

public:
	static Map <Model, LongWord> & GetModels();

	static void AddModel(Model*, const char*);
};
