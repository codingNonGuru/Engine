#include "DataBuffer.hpp"
#include "ShaderManager.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Mesh.hpp"
#include "MeshManager.hpp"

#include "Game/SettlementRenderer.hpp"
#include "Game/WorldScene.hpp"
#include "Game/World.hpp"
#include "Game/Tile.hpp"
#include "Game/Settlement.hpp"
#include "Game/Types.hpp"

SettlementRenderer* SettlementRenderer::instance_ = nullptr;

const int SettlementRenderer::BUILDING_RENDER_CAPACITY = 1048576;

enum class Shaders {BUILDING};

SettlementRenderer* SettlementRenderer::GetInstance()
{
	if(instance_ == nullptr)
		instance_ = new SettlementRenderer();

	return instance_;
}

SettlementRenderer::SettlementRenderer()
{
	shaders_.Initialize(1);

	auto shader = shaders_.Add(Shaders::BUILDING);
	*shader = ShaderManager::GetShader("Building");

	buffers_.Initialize(SettlementModelBuffers::COUNT);

	mesh_ = MeshManager::GetMesh("Building");

	auto meshAttributes = mesh_->GetAttributes();

	auto attributeData = meshAttributes.Get("position")->GetData();
	*buffers_.Add(SettlementModelBuffers::VERTEX_POSITIONS) = new DataBuffer(attributeData->GetMemoryCapacity(), attributeData->GetData());

	attributeData = meshAttributes.Get("normal")->GetData();
	*buffers_.Add(SettlementModelBuffers::VERTEX_NORMALS) = new DataBuffer(attributeData->GetMemoryCapacity(), attributeData->GetData());

	attributeData = meshAttributes.Get("index")->GetData();
	*buffers_.Add(SettlementModelBuffers::INDICES) = new DataBuffer(attributeData->GetMemoryCapacity(), attributeData->GetData());

	auto buffer = buffers_.Add(SettlementModelBuffers::BUILDING_INDICES);
	*buffer = new DataBuffer(sizeof(Index) * BUILDING_RENDER_CAPACITY, nullptr);

	buildingIndices_.Initialize(BUILDING_RENDER_CAPACITY);
}

void SettlementRenderer::Render(Camera* camera)
{
	auto world = WorldScene::GetWorld();

	buildingIndices_.Reset();

	auto centralPosition = camera->GetTarget();
	auto & tiles = world->GetTiles();
	int range = 30;
	for(int x = centralPosition.x - range; x < centralPosition.x + range; ++x)
	{
		for(int y = centralPosition.y - range; y < centralPosition.y + range; ++y)
		{
			auto tile = tiles.Get(x, y);
			auto settlement = tile->GetSettlement();
			if(settlement == nullptr)
				continue;

			auto renderData = settlement->GetRenderData();
			for(auto index = renderData->BuildingIndex_; index < renderData->BuildingIndex_ + renderData->BuildingCount_; ++index)
			{
				auto indexPointer = buildingIndices_.Allocate();
				if(indexPointer == nullptr)
					continue;

				*indexPointer = index;
			}
		}
	}

	auto buildingShader = *shaders_.Get(Shaders::BUILDING);
	if(buildingShader == nullptr)
		return;

	buildingShader->Bind();

	DataBuffer* buffer = nullptr;

	buffer = *buffers_.Get(SettlementModelBuffers::VERTEX_POSITIONS);
	buffer->Bind(0);

	buffer = *buffers_.Get(SettlementModelBuffers::VERTEX_NORMALS);
	buffer->Bind(1);

	buffer = *buffers_.Get(SettlementModelBuffers::INDICES);
	buffer->Bind(2);

	buffer = *buffers_.Get(SettlementModelBuffers::BUILDING_INDICES);
	buffer->Bind(3);
	buffer->UploadData(buildingIndices_.GetStart(), buildingIndices_.GetMemorySize());

	buffer = *buffers_.Get(SettlementModelBuffers::BUILDING_DATAS);
	buffer->Bind(4);

	buildingShader->SetConstant(camera->GetMatrix(), "viewMatrix");
	buildingShader->SetConstant(mesh_->GetIndexCount(), "indexCount");
	buildingShader->SetConstant(camera->GetPosition(), "cameraPosition");

	//std::cout<<"--------------------------> "<<buildingIndices_.GetSize() * mesh_->GetIndexCount()<<"\n";

	glDrawArrays(GL_TRIANGLES, 0, buildingIndices_.GetSize() * mesh_->GetIndexCount());

	buildingShader->Unbind();
}

void SettlementRenderer::Update(Camera* camera)
{
	GetInstance()->Render(camera);
}

Array <SettlementRenderData> & SettlementRenderer::GetSettlementDatas()
{
	return GetInstance()->settlementDatas_;
}

Array <BuildingRenderData> & SettlementRenderer::GetBuildingDatas()
{
	return GetInstance()->buildingDatas_;
}

Map <DataBuffer*> & SettlementRenderer::GetBuffers()
{
	return GetInstance()->buffers_;
}
