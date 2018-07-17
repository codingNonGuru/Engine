#pragma once

#include "Conventions.hpp"

class Model;

class ModelManager
{
private:
	static Map <Model, Word> models_;

public:
	static Map <Model, Word> & GetModels();

	static void AddModel(Model*, const char*);
};
