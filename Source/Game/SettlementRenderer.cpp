#include "DataBuffer.hpp"
#include "ShaderManager.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Mesh.hpp"
#include "MeshManager.hpp"
#include "BufferManager.hpp"
#include "Light.hpp"
#include "FrameBuffer.hpp"
#include "RenderManager.hpp"

#include "Game/SettlementRenderer.hpp"
#include "Game/WorldScene.hpp"
#include "Game/World.hpp"
#include "Game/Tile.hpp"
#include "Game/Settlement.hpp"
#include "Game/Types.hpp"

SettlementRenderer* SettlementRenderer::instance_ = nullptr;

const int SettlementRenderer::BUILDING_RENDER_CAPACITY = 32768;

enum class Shaders {BUILDING, BUILDING_SHADOW};

SettlementRenderer* SettlementRenderer::GetInstance()
{
	if(instance_ == nullptr)
		instance_ = new SettlementRenderer();

	return instance_;
}

SettlementRenderer::SettlementRenderer()
{
	shaders_.Initialize(2);

	auto shader = shaders_.Add(Shaders::BUILDING);
	*shader = ShaderManager::GetShader("Building");

	shader = shaders_.Add(Shaders::BUILDING_SHADOW);
	*shader = ShaderManager::GetShader("BuildingShadow");

	buffers_.Initialize(SettlementModelBuffers::COUNT);

	AssembleMesh();

	auto buffer = buffers_.Add(SettlementModelBuffers::BUILDING_INDICES);
	*buffer = new DataBuffer(sizeof(Index) * BUILDING_RENDER_CAPACITY, nullptr);

	textures_.Initialize(SettlementModelTextures::COUNT);

	auto shadowFrameBuffer = BufferManager::GetFrameBuffer("shadow");
	auto texture = shadowFrameBuffer->GetDepthTexture();
	*textures_.Add(SettlementModelTextures::SHADOW_MAP) = texture;

	buildingIndices_.Initialize(BUILDING_RENDER_CAPACITY);
}

void SettlementRenderer::AssembleMesh()
{
	meshes_.Initialize(7);

	*meshes_.Add(0) = MeshManager::GetMesh("Building0");
	*meshes_.Add(1) = MeshManager::GetMesh("Building1");
	*meshes_.Add(2) = MeshManager::GetMesh("Building5");
	*meshes_.Add(3) = MeshManager::GetMesh("Building2");
	*meshes_.Add(4) = MeshManager::GetMesh("Building6");
	*meshes_.Add(5) = MeshManager::GetMesh("Building3");
	*meshes_.Add(6) = MeshManager::GetMesh("Building4");

	Length positionCapacity = 0;
	Length normalCapacity = 0;
	Length indexCapacity = 0;
	Length textureIndexCapacity = 0;

	defaultMeshSize_ = 0;

	for(auto meshIterator = meshes_.GetStart(); meshIterator != meshes_.GetEnd(); ++meshIterator)
	{
		auto mesh = *meshIterator;

		auto & meshAttributes = mesh->GetAttributes();

		auto attributeData = meshAttributes.Get("position")->GetData();
		positionCapacity += attributeData->GetMemoryCapacity();

		attributeData = meshAttributes.Get("normal")->GetData();
		normalCapacity += attributeData->GetMemoryCapacity();

		attributeData = meshAttributes.Get("index")->GetData();
		indexCapacity += attributeData->GetMemoryCapacity();

		attributeData = meshAttributes.Get("textureIndex")->GetData();
		textureIndexCapacity += attributeData->GetMemoryCapacity();

		if(mesh->GetIndexCount() > defaultMeshSize_)
			defaultMeshSize_ = mesh->GetIndexCount();
	}

	auto positionData = new Byte[positionCapacity];
	auto normalData = new Byte[normalCapacity];
	auto indexData = new Byte[indexCapacity];
	auto textureIndexData = new Byte[textureIndexCapacity];

	Index positionIndex = 0;
	Index normalIndex = 0;
	Index indexIndex = 0;
	Index textureIndexIndex = 0;

	for(auto meshIterator = meshes_.GetStart(); meshIterator != meshes_.GetEnd(); ++meshIterator)
	{
		auto mesh = *meshIterator;

		auto & meshAttributes = mesh->GetAttributes();

		Index lastPositionIndex = positionIndex;

		auto attributeData = meshAttributes.Get("position")->GetData();
		CopyMemory(positionData + positionIndex, attributeData->GetData(), attributeData->GetMemoryCapacity());
		positionIndex += attributeData->GetMemoryCapacity();

		attributeData = meshAttributes.Get("normal")->GetData();
		CopyMemory(normalData + normalIndex, attributeData->GetData(), attributeData->GetMemoryCapacity());
		normalIndex += attributeData->GetMemoryCapacity();

		attributeData = meshAttributes.Get("index")->GetData();
		CopyMemory(indexData + indexIndex, attributeData->GetData(), attributeData->GetMemoryCapacity());

		for(Index* index = (Index*)indexData + indexIndex / 4; index != (Index*)indexData + indexCapacity / 4; index++)
		{
			*index = *index + lastPositionIndex / 12;
		}

		indexIndex += attributeData->GetMemoryCapacity();

		attributeData = meshAttributes.Get("textureIndex")->GetData();
		CopyMemory(textureIndexData + textureIndexIndex, attributeData->GetData(), attributeData->GetMemoryCapacity());
		textureIndexIndex += attributeData->GetMemoryCapacity();
	}

	*buffers_.Add(SettlementModelBuffers::VERTEX_POSITIONS) = new DataBuffer(positionCapacity, positionData);

	*buffers_.Add(SettlementModelBuffers::VERTEX_NORMALS) = new DataBuffer(normalCapacity, normalData);

	*buffers_.Add(SettlementModelBuffers::INDICES) = new DataBuffer(indexCapacity, indexData);

	*buffers_.Add(SettlementModelBuffers::TEXTURE_INDICES) = new DataBuffer(textureIndexCapacity, textureIndexData);
}

void SettlementRenderer::ProcessData(Camera* camera)
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

	DataBuffer* buffer = *buffers_.Get(SettlementModelBuffers::BUILDING_INDICES);
	buffer->Bind(3);
	buffer->UploadData(buildingIndices_.GetStart(), buildingIndices_.GetMemorySize());
}

void SettlementRenderer::RenderShadows(Camera* camera, Light* light)
{
	auto frameBuffer = BufferManager::GetFrameBuffer("shadow");
	if(frameBuffer != nullptr)
	{
		frameBuffer->BindBuffer();
	}

	glClearDepth(1.0f);
	glClear(GL_DEPTH_BUFFER_BIT);

	auto depthMatrix = light->GetShadowMatrix(camera->GetViewDistance() * RenderManager::SHADOW_MAP_SIZE_MODIFIER, camera->GetTarget());

	ProcessData(camera);

	auto buildingShader = *shaders_.Get(Shaders::BUILDING_SHADOW);
	if(buildingShader == nullptr)
		return;

	buildingShader->Bind();

	DataBuffer* buffer = nullptr;

	buffer = *buffers_.Get(SettlementModelBuffers::VERTEX_POSITIONS);
	buffer->Bind(0);

	buffer = *buffers_.Get(SettlementModelBuffers::INDICES);
	buffer->Bind(2);

	buffer = *buffers_.Get(SettlementModelBuffers::BUILDING_INDICES);
	buffer->Bind(3);

	buffer = *buffers_.Get(SettlementModelBuffers::BUILDING_DATAS);
	buffer->Bind(4);

	buildingShader->SetConstant(depthMatrix, "depthMatrix");
	buildingShader->SetConstant(defaultMeshSize_, "indexCount");

	glDrawArrays(GL_TRIANGLES, 0, buildingIndices_.GetSize() * defaultMeshSize_);

	buildingShader->Unbind();

	frameBuffer = BufferManager::GetFrameBuffer("default");
	if(frameBuffer != nullptr)
	{
		frameBuffer->BindBuffer();
	}
}

void SettlementRenderer::Render(Camera* camera, Light* light)
{
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

	buffer = *buffers_.Get(SettlementModelBuffers::BUILDING_DATAS);
	buffer->Bind(4);

	buffer = *buffers_.Get(SettlementModelBuffers::TEXTURE_INDICES);
	buffer->Bind(5);

	Texture* texture = *textures_.Get(SettlementModelTextures::SHADOW_MAP);
	buildingShader->BindTexture(texture, "shadowMap");

	buildingShader->SetConstant(camera->GetMatrix(), "viewMatrix");
	auto depthMatrix = light->GetShadowMatrix(camera->GetViewDistance() * RenderManager::SHADOW_MAP_SIZE_MODIFIER, camera->GetTarget());
	buildingShader->SetConstant(depthMatrix, "depthMatrix");
	buildingShader->SetConstant(defaultMeshSize_, "indexCount");
	buildingShader->SetConstant(camera->GetPosition(), "cameraPosition");

	glDrawArrays(GL_TRIANGLES, 0, buildingIndices_.GetSize() * defaultMeshSize_);

	buildingShader->Unbind();
}

void SettlementRenderer::Update(Camera* camera, Light* light)
{
	GetInstance()->Render(camera, light);
}

void SettlementRenderer::ProjectShadows(Camera* camera, Light* light)
{
	GetInstance()->RenderShadows(camera, light);
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

Mesh* SettlementRenderer::GetMesh(Index index)
{
	auto mesh = GetInstance()->meshes_.Get(index);
	if(mesh == nullptr)
		return nullptr;

	return *mesh;
}
