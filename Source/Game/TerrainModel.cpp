#include "DataBuffer.hpp"
#include "ShaderManager.hpp"
#include "Shader.hpp"
#include "Camera.hpp"

#include "Game/TerrainModel.hpp"
#include "Game/ReliefGenerator.hpp"
#include "Game/Types.hpp"
#include "Game/WorldScene.hpp"

enum class Shaders {DISPLACE, RENDER};

TerrainModel::TerrainModel()
{
	auto world = WorldScene::GetWorld();
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
	*buffers_.Add(TerrainModelBuffers::POSITION_OUTPUT) = buffer;

	buffer = new DataBuffer(bufferSize);
	*buffers_.Add(TerrainModelBuffers::TANGENT_OUTPUT) = buffer;

	buffer = new DataBuffer(bufferSize);
	*buffers_.Add(TerrainModelBuffers::NORMAL_OUTPUT) = buffer;

	shaders_.Initialize(2);

	auto shader = shaders_.Add(Shaders::DISPLACE);
	*shader = ShaderManager::GetShader("Displace");

	shader = shaders_.Add(Shaders::RENDER);
	*shader = ShaderManager::GetShader("Land");

	textures_.Initialize(TerrainModelTextures::COUNT);

	auto texture = ReliefGenerator::GetModelTexture(TerrainModelTextures::BASE_HEIGHT);
	*textures_.Add(TerrainModelTextures::BASE_HEIGHT) = texture;

	texture = ReliefGenerator::GetModelTexture(TerrainModelTextures::DETAIL_HEIGHT);
	*textures_.Add(TerrainModelTextures::DETAIL_HEIGHT) = texture;
}

void TerrainModel::Initialize()
{

}

void TerrainModel::Render(Camera* camera)
{
	auto displaceShader = *shaders_.Get(Shaders::DISPLACE);
	if(displaceShader == nullptr)
		return;

	displaceShader->Bind();

	DataBuffer* buffer = nullptr;

	buffer = *buffers_.Get(TerrainModelBuffers::INDEX_LINKS);
	buffer->Bind(0);

	buffer = *buffers_.Get(TerrainModelBuffers::POSITION_INPUT);
	buffer->Bind(1);
	Length vertexCount = buffer->GetSize();

	buffer = *buffers_.Get(TerrainModelBuffers::POSITION_OUTPUT);
	buffer->Bind(2);

	buffer = *buffers_.Get(TerrainModelBuffers::NORMAL_OUTPUT);
	buffer->Bind(3);

	buffer = *buffers_.Get(TerrainModelBuffers::TANGENT_OUTPUT);
	buffer->Bind(4);

	Texture* texture = nullptr;

	texture = *textures_.Get(TerrainModelTextures::BASE_HEIGHT);
	displaceShader->BindTexture(texture, "reliefHeightMap");

	texture = *textures_.Get(TerrainModelTextures::DETAIL_HEIGHT);
	displaceShader->BindTexture(texture, "reliefDetailMap");

	auto viewDistance = camera->GetViewDistance();
	displaceShader->SetConstant(viewDistance, "size");

	auto center = camera->GetPosition();
	displaceShader->SetConstant(Float2(center.x, center.y), "center");
	displaceShader->SetConstant(vertexCount, "vertexCount");

	auto azimuth = camera->GetAzimuth();
	displaceShader->SetConstant((float)cos(azimuth), "c");
	displaceShader->SetConstant((float)sin(azimuth), "s");

	Length workSize = vertexCount / 32 + 1;

	displaceShader->SetConstant(0, "mode");
	displaceShader->DispatchCompute(workSize);

	displaceShader->SetConstant(1, "mode");
	displaceShader->DispatchCompute(workSize);

	displaceShader->Unbind();

	auto renderShader = *shaders_.Get(Shaders::RENDER);
	if(renderShader == nullptr)
		return;

	renderShader->Bind();

	buffer = *buffers_.Get(TerrainModelBuffers::POSITION_OUTPUT);
	buffer->Bind(0);

	buffer = *buffers_.Get(TerrainModelBuffers::INDICES);
	buffer->Bind(1);
	Length indexCount = buffer->GetSize();

	buffer = *buffers_.Get(TerrainModelBuffers::NORMAL_OUTPUT);
	buffer->Bind(2);

	buffer = *buffers_.Get(TerrainModelBuffers::TANGENT_OUTPUT);
	buffer->Bind(3);

	renderShader->SetConstant(camera->GetMatrix(), "projMatrix");
	renderShader->SetConstant(camera->GetPosition(), "cameraPos");

	glDrawArrays(GL_TRIANGLES, 0, indexCount);

	DEBUG_OPENGL

	renderShader->Unbind();
}
