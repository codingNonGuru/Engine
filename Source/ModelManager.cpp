#include "ModelManager.hpp"

#include "Model.hpp"

Map <Model, LongWord> ModelManager::models_ = Map <Model, LongWord> ();

#define MAXIMUM_MODEL_COUNT 256

void ModelManager::Initialize()
{
	models_.Initialize(MAXIMUM_MODEL_COUNT);
}

Map <Model, LongWord> & ModelManager::GetModels()
{
	return models_;
}

void ModelManager::AddModel(Model* model, const char* name)
{
	auto newModel = models_.Allocate(LongWord(name));
	if(newModel == nullptr)
	{
		std::cout<<"Maximum model count was reached.\n";
		return;
	}

	*newModel = *model;
}




