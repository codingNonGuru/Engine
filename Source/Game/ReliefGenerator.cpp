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

enum class Buffers
{
	TERRAIN, CARVE, RELIEF, GRADIENT, KERNEL, PARTICLE, PARTICLE_VELOCITY
};

Map <DataBuffer, Buffers> buffers = Map <DataBuffer, Buffers> (16);

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
