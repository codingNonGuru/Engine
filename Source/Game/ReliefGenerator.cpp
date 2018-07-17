#include "Shader.hpp"
#include "ShaderManager.hpp"
#include "DataBuffer.hpp"
#include "Texture.hpp"
#include "TextureManager.hpp"
#include "Utility/Utility.hpp"
#include "Utility/Perlin.hpp"
#include "Utility/Kernel.hpp"

#include "Game/ReliefGenerator.hpp"
#include "Game/WorldGenerator.hpp"
#include "Game/WorldParameterSet.hpp"
#include "Game/World.hpp"
#include "Game/Tile.hpp"
#include "Game/Types.hpp"

enum class Buffers
{
	TERRAIN, CARVE, RELIEF, GRADIENT, KERNEL, PARTICLE, PARTICLE_VELOCITY
};

Map <DataBuffer, Buffers> buffers = Map <DataBuffer, Buffers> (16);

Map <DataBuffer*, LongWord> ReliefGenerator::modelBuffers_ = Map <DataBuffer*, LongWord> (TerrainModelBuffers::COUNT);

Shader* shader = nullptr;

void ReliefGenerator::Generate(World& world)
{
	Size size = world.GetSize();

	SetupBuffers(world);

	shader = ShaderManager::GetShader("GenerateRelief");
	if(!shader)
		return;

	shader->Bind();

	buffers.Get(Buffers::TERRAIN)->Bind(0);
	Perlin::GetResultBuffer()->Bind(1);
	buffers.Get(Buffers::CARVE)->Bind(2);
	buffers.Get(Buffers::RELIEF)->Bind(3);
	buffers.Get(Buffers::GRADIENT)->Bind(9);
	buffers.Get(Buffers::KERNEL)->Bind(10);
	buffers.Get(Buffers::PARTICLE)->Bind(11);
	buffers.Get(Buffers::PARTICLE_VELOCITY)->Bind(12);

	auto computeSize = size / 4;

	shader->SetConstant(size, "size");

	shader->SetConstant(0, "mode");
	shader->DispatchCompute(computeSize);

	float continentLift = size.x / 10;
	float islandLift = size.y / 20;

	Float2 position;

	position = (Float2)size * Float2(0.25f, 0.4f);
	LiftTerrain(position, continentLift, computeSize);
	position = (Float2)size * Float2(0.25f, 0.25f);
	LiftTerrain(position, continentLift, computeSize);

	position = (Float2)size * Float2(0.75f, 0.6f);
	LiftTerrain(position, continentLift, computeSize);
	position = (Float2)size * Float2(0.75f, 0.75f);
	LiftTerrain(position, continentLift, computeSize);

	position = (Float2)size * Float2(0.6f, 0.25f);
	LiftTerrain(position, islandLift, computeSize);

	position = (Float2)size * Float2(0.4f, 0.75f);
	LiftTerrain(position, islandLift, computeSize);

	for(int i = 0; i < 24; ++i)
	{
		shader->SetConstant(2, "mode");
		shader->DispatchCompute(computeSize);
	}

	shader->SetConstant(3, "mode");
	shader->DispatchCompute(computeSize);

	/*
	for(int erodePass = 0; erodePass < 1; ++erodePass) {
		glUniform1ui(0, 4);
		glDispatchCompute(width_ / workGroupSize, height_ / workGroupSize, 1);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
		for(int flowPass = 0; flowPass < 200; ++flowPass) {
			glUniform1ui(0, 10);
			glUniform2i(4, particles.getWidth(), particles.getHeight());
			glDispatchCompute(particles.getWidth(), particles.getHeight(), 1);
			glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
			engine_->getShader(Shaders::GENERATE_TERRAIN).unuse();

			glClearColor(0.5f, 0.1f, 0.0f, 1.0f);
			glClearDepth(1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			engine_->getShader(Shaders::BASIC).use();
			glBindBuffer(GL_ARRAY_BUFFER, particleBuffer);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

			glUniformMatrix4fv(0, 1, GL_FALSE, &finalMatrix[0][0]);
			glPointSize(1.0f);
			glDrawArrays(GL_POINTS, 0, particles.getWidth() * particles.getHeight());
			engine_->getShader(Shaders::BASIC).unuse();

			SDL_GL_SwapWindow(engine_->getWindow());

			engine_->getShader(Shaders::GENERATE_TERRAIN).use();
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleBuffer);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 11, particleBuffer);
		}
		glFinish();
	}*/

	shader->Unbind();

	Grid <Float> terrain(size.x, size.y);
	buffers.Get(Buffers::TERRAIN)->Download(&terrain);

	Grid <Integer> reliefs(size.x, size.y);
	buffers.Get(Buffers::RELIEF)->Download(&reliefs);

	Grid<Byte4> previewData(size.x, size.y);
	for(int x = 0; x < previewData.GetWidth(); ++x)
	{
		int sum = 0;
		for(int y = 0; y < previewData.GetHeight(); ++y)
		{
			auto previewPixel = previewData(x, y);
			auto height = *reliefs(x, y) == 1 ? Byte(255) : Byte(0);
			*previewPixel = Byte4(128, 0, 0, height);
			//*previewPixel = Byte4(128, 0, 0, *terrain(x, y) * 100.0f);
		}
	}

	auto previewTexture = WorldGenerator::GetReliefPreview();
	previewTexture->Upload(previewData.GetData());

	for(auto buffer = buffers.GetStart(); buffer != buffers.GetEnd(); ++buffer)
	{
		if(buffer == buffers.Get(Buffers::TERRAIN))
			continue;

		buffer->Delete();
	}

	auto& tiles = world.GetTiles();
	tiles.Initialize(size.x, size.y);

	for(int x = 0; x < tiles.GetWidth(); ++x)
	{
		for(int y = 0; y < tiles.GetHeight(); ++y)
		{
			auto& position = tiles(x, y)->GetPosition();
			position.x = (float)x;
			position.y = (float)y;
			position.z = *terrain(x, y);

			auto& relief = tiles(x, y)->GetRelief();
			relief = (ReliefTypes)*reliefs(x, y);
		}
	}
}

DataBuffer* ReliefGenerator::GetFinalBuffer()
{
	return buffers.Get(Buffers::TERRAIN);
}

DataBuffer* ReliefGenerator::GetModelBuffer(LongWord identifier)
{
	auto bufferPointer = modelBuffers_.Get(identifier);
	if(bufferPointer == nullptr)
		return nullptr;

	return *bufferPointer;
}

void ReliefGenerator::SetupBuffers(World& world)
{
	auto size = world.GetSize();
	auto area = size.x * size.y;

	DataBuffer* buffer = nullptr;

	buffer = buffers.Add(Buffers::TERRAIN);
	if(buffer)
	{
		buffer->Generate(area * sizeof(Float));
	}

	buffer = buffers.Add(Buffers::CARVE);
	if(buffer)
	{
		buffer->Generate(area * sizeof(Float));
	}

	buffer = buffers.Add(Buffers::RELIEF);
	if(buffer)
	{
		buffer->Generate(area * sizeof(Integer));
	}

	buffer = buffers.Add(Buffers::GRADIENT);
	if(buffer)
	{
		buffer->Generate(area * sizeof(Float) * 2);
	}

	auto kernel = new Kernel(7);
	kernel->Initialize(KernelTypes::GAUSS, 32.0f);

	buffer = buffers.Add(Buffers::KERNEL);
	if(buffer)
	{
		buffer->Generate(kernel->GetValues().GetMemorySize(), kernel->GetValues().GetStart());
	}

	GLuint finalBuffer, carveBuffer, gradientBuffer, filterBuffer, particleBuffer, particleVelocityBuffer;

	container::Grid <Float2> particles(size.x / 64, size.y / 64);
	for(int x = 0; x < particles.GetWidth(); ++x)
	{
		for(int y = 0; y < particles.GetHeight(); ++y)
		{
			float posX = float(x) * 64.0f + utility::GetRandom(16.0f, 48.0f);
			float posY = float(y) * 64.0f + utility::GetRandom(16.0f, 48.0f);

			*particles(x, y) = Float2(posX, posY);
		}
	}

	buffer = buffers.Add(Buffers::PARTICLE);
	if(buffer)
	{
		buffer->Generate(particles.GetMemorySize(), particles.GetStart());
	}

	container::Grid <Float2> particleVelocities(size.x, size.y);
	for(int x = 0; x < particleVelocities.GetWidth(); ++x)
		for(int y = 0; y < particleVelocities.GetHeight(); ++y)
		{
			*particleVelocities(x, y) = Float2(0.0f, 0.0f);
		}

	buffer = buffers.Add(Buffers::PARTICLE_VELOCITY);
	if(buffer)
	{
		buffer->Generate(particleVelocities.GetMemorySize(), particleVelocities.GetStart());
	}

	container::Grid <float> perlinDetail(size.x, size.y);
	Perlin::Generate(size, Range(0.0f, 1.0f), 3.5f, 1.5f, 0.5f, 2.0f);
}

void ReliefGenerator::LiftTerrain(Float2 position, Float decay, Size computeSize)
{
	shader->SetConstant(position, "target");
	shader->SetConstant(decay, "decay");

	shader->SetConstant(1, "mode");
	shader->DispatchCompute(computeSize);
}

void ReliefGenerator::GenerateModel(World& world)
{
	struct LinkBatch
	{
		Index links_[12];
		unsigned int count_;

		LinkBatch()
		{
			count_ = 0;
		}

		void Add(Index link)
		{
			links_[count_] = link;
			count_++;
		}
	};

	Index capacity = 4096 * 256;
	Array <Float3> vertexPositions(capacity);
	Array <Index> indices(16384 * 256);
	Array <unsigned int> indexSeries(16384);
	Array <LinkBatch> linkBatches(capacity);
	Array <unsigned int> links(capacity * 12);

	for(int i = 0; i < linkBatches.GetCapacity(); ++i)
		linkBatches.Allocate()->count_ = 0;

	float horIncr = 0.01f;
	float vertIncr = 0.866f * horIncr;

	bool flip = true;
	Index index = 0;

	*indexSeries.Allocate() = index;
	for(float y = -5.0f; y < 5.0f; y += vertIncr)
	{
		for(float x = -3.0f + (flip ? 0.0f : horIncr * 0.5f); x < 3.0f; x += horIncr)
		{
			*vertexPositions.Allocate() = Float3(x, y, 0.0f);
			index++;
		}
		flip = flip ? false : true;
		*indexSeries.Allocate() = index;
	}

	for(int i = 0; i < indexSeries.GetSize() - 3; i += 2)
	{
		bool first = false;
		int j = *indexSeries.Get(i);
		int l = *indexSeries.Get(i + 2);
		for(int k = *indexSeries.Get(i + 1); k < *indexSeries.Get(i + 2) - 3; ++j, ++k, ++l)
		{
			*indices.Allocate() = j;
			*indices.Allocate() = j + 1;
			*indices.Allocate() = k;

			linkBatches.Get(j)->Add(j + 1);
			linkBatches.Get(j)->Add(k);
			linkBatches.Get(j + 1)->Add(j);
			linkBatches.Get(j + 1)->Add(k);
			linkBatches.Get(k)->Add(j);
			linkBatches.Get(k)->Add(j + 1);

			*indices.Allocate() = j + 1;
			*indices.Allocate() = k;
			*indices.Allocate() = k + 1;

			linkBatches.Get(j + 1)->Add(k);
			linkBatches.Get(j + 1)->Add(k + 1);
			linkBatches.Get(k)->Add(j + 1);
			linkBatches.Get(k)->Add(k + 1);
			linkBatches.Get(k + 1)->Add(j + 1);
			linkBatches.Get(k + 1)->Add(k);

			*indices.Allocate() = l;
			*indices.Allocate() = k;
			*indices.Allocate() = l + 1;

			linkBatches.Get(l)->Add(k);
			linkBatches.Get(l)->Add(l + 1);
			linkBatches.Get(k)->Add(l);
			linkBatches.Get(k)->Add(l + 1);
			linkBatches.Get(l + 1)->Add(l);
			linkBatches.Get(l + 1)->Add(k);

			*indices.Allocate() = l + 1;
			*indices.Allocate() = k;
			*indices.Allocate() = k + 1;

			linkBatches.Get(l + 1)->Add(k);
			linkBatches.Get(l + 1)->Add(k + 1);
			linkBatches.Get(k)->Add(l + 1);
			linkBatches.Get(k)->Add(k + 1);
			linkBatches.Get(k + 1)->Add(l + l);
			linkBatches.Get(k + 1)->Add(k);
		}
	}

	for(int i = 0; i < linkBatches.GetSize(); ++i)
	{
		for(int j = 0; j < 12; ++j)
			*links.Allocate() = linkBatches.Get(i)->links_[j];
	}

	auto positionInputBuffer = modelBuffers_.Add(TerrainModelBuffers::POSITION_INPUT);
	*positionInputBuffer = new DataBuffer(vertexPositions.GetMemorySize(), vertexPositions.GetStart());
	(*positionInputBuffer)->SetSize(vertexPositions.GetSize());

	auto indexLinkBuffer = modelBuffers_.Add(TerrainModelBuffers::INDEX_LINKS);
	*indexLinkBuffer = new DataBuffer(links.GetMemorySize(), links.GetStart());

	auto indexBuffer = modelBuffers_.Add(TerrainModelBuffers::INDICES);
	*indexBuffer = new DataBuffer(indices.GetMemorySize(), indices.GetStart());

	/*int i = 0;
	for(auto l = links.getStart(); l != links.getEnd(); ++l) {
		std::cout<<*l<<" ";
		if(i % 6 == 5)
			std::cout<<"\n";
	}*/

	/*container::Array<glm::vec3> vertexPositions(4096 * 64);
	container::Array<unsigned int> indices(16384 * 64);
	int order = 8;
	float increment = 1.0f / pow(2.0f, (float)order);
	float offset = increment / 2.0f;
	for(int x = 0; x <= pow(2, order); ++x)
		for(int y = 0; y <= pow(2, order); ++y)
			*vertexPositions.Allocate() = glm::vec3((float)x * increment - 0.5f, (float)y * increment - 0.5f, 0.0f);
	for(int x = 0; x < pow(2, order); ++x)
		for(int y = 0; y < pow(2, order); ++y)
			*vertexPositions.Allocate() = glm::vec3(offset + (float)x * increment - 0.5f, offset + (float)y * increment - 0.5f, 0.0f);
	int middleStart = (pow(2, order) + 1) * (pow(2, order) + 1);
	for(int x = 0; x < pow(2, order); ++x)
		for(int y = 0; y < pow(2, order); ++y) {
			int index = x * pow(2, order) + y;
			int middle = middleStart + index;
			int topLeft = index + x;
			int topRight = topLeft + 1;
			int bottomLeft = topLeft + pow(2, order) + 1;
			int bottomRight = bottomLeft + 1;
			*indices.Allocate() = middle;
			*indices.Allocate() = topLeft;
			*indices.Allocate() = topRight;

			*indices.Allocate() = middle;
			*indices.Allocate() = topRight;
			*indices.Allocate() = bottomRight;

			*indices.Allocate() = middle;
			*indices.Allocate() = bottomRight;
			*indices.Allocate() = bottomLeft;

			*indices.Allocate() = middle;
			*indices.Allocate() = bottomLeft;
			*indices.Allocate() = topLeft;
		}*/

	auto & worldTiles = world.GetTiles();
	Grid <Float> heightMap (worldTiles.GetWidth(), worldTiles.GetHeight());
	for(int x = 0; x < heightMap.GetWidth(); ++x)
		for(int y = 0; y < heightMap.GetHeight(); ++y)
		{
			//*heightMap(x, y) = (worldTiles.Get(x, y)->position_.z - world.averageHeight_) * 1.0f + world.averageHeight_;
			auto & position = worldTiles.Get(x, y)->GetPosition();
			*heightMap(x, y) = position.z;
		}

	auto texture = new Texture(world.GetSize(), TextureFormats::ONE_FLOAT, &heightMap);

	Grid <Float> detailMap(worldTiles.GetWidth(), worldTiles.GetHeight());
	Perlin::Generate(world.GetSize(), Range(0.0f, 1.0f), 0.0f, 2.0f, 0.5f, 1.0f);

	Perlin::Download(&detailMap);
	texture = new Texture(world.GetSize(), TextureFormats::ONE_FLOAT, &detailMap);
}
