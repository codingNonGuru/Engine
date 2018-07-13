#include "ModelManager.hpp"

#include "Model.hpp"

#define MAXIMUM_MODEL_COUNT 256

Map <Model, LongWord> ModelManager::models_ = Map <Model, LongWord> (MAXIMUM_MODEL_COUNT);

Map <Model, LongWord> & ModelManager::GetModels()
{
	return models_;
}

void ModelManager::AddModel(Model* model, const char* name)
{
	auto newModel = models_.Add(name);
	if(newModel == nullptr)
	{
		std::cout<<"Maximum model count was reached.\n";
		return;
	}

	*newModel = *model;
}




