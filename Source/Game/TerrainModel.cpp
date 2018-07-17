#include "DataBuffer.hpp"

#include "Game/TerrainModel.hpp"
#include "Game/ReliefGenerator.hpp"
#include "Game/Types.hpp"
#include "Game/WorldScene.hpp"

TerrainModel::TerrainModel()
{
	/*auto world = WorldScene::GetWorld();
	ReliefGenerator::GenerateModel(*world);

	DataBuffer* buffer = nullptr;
	buffers_.Initialize(TerrainModelBuffers::COUNT);

	buffer = ReliefGenerator::GetModelBuffer(TerrainModelBuffers::POSITION_INPUT);
	*buffers_.Add(TerrainModelBuffers::POSITION_INPUT) = buffer;
	auto bufferSize = buffer->GetMemorySize();

	buffer = ReliefGenerator::GetModelBuffer(TerrainModelBuffers::INDEX_LINKS);
	*buffers_.Add(TerrainModelBuffers::INDEX_LINKS) = buffer;

	buffer = ReliefGenerator::GetModelBuffer(TerrainModelBuffers::INDICES);
	*buffers_.Add(TerrainModelBuffers::INDICES) = buffer;

	buffer = new DataBuffer(bufferSize);
	*buffers_.Add(TerrainModelBuffers::TANGENT_OUTPUT) = buffer;

	buffer = new DataBuffer(bufferSize);
	*buffers_.Add(TerrainModelBuffers::NORMAL_OUTPUT) = buffer;*/
}

void TerrainModel::Initialize()
{

}

void TerrainModel::Render(Camera* camera)
{
}
