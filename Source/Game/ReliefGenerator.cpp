#include "Shader.hpp"
#include "ShaderManager.hpp"
#include "DataBuffer.hpp"
#include "Utility/Utility.hpp"
#include "Utility/Perlin.hpp"
#include "Utility/Kernel.hpp"

#include "Game/ReliefGenerator.hpp"
#include "Game/WorldParameterSet.hpp"

enum class Buffers
{
	FINAL, CARVE, GRADIENT, KERNEL, PARTICLE, PARTICLE_VELOCITY
};

Map <DataBuffer, Buffers> buffers = Map <DataBuffer, Buffers> (16);

Shader* shader = nullptr;

void ReliefGenerator::Generate(World& world, const WorldParameterSet& parameterSet)
{
	SetupBuffers(world, parameterSet);

	shader = ShaderManager::GetShaderMap().Get("GenerateRelief");
	if(!shader)
		return;

	shader->Bind();

	buffers.Get(Buffers::FINAL)->Bind(0);
	Perlin::GetResultBuffer()->Bind(1);
	buffers.Get(Buffers::CARVE)->Bind(2);
	buffers.Get(Buffers::GRADIENT)->Bind(9);
	buffers.Get(Buffers::KERNEL)->Bind(10);
	buffers.Get(Buffers::PARTICLE)->Bind(11);
	buffers.Get(Buffers::PARTICLE_VELOCITY)->Bind(12);

	/*glUniform2i(1, width_, height_);
	int workGroupSize = 1;

	glUniform1ui(0, 0);
	glDispatchCompute(width_ / workGroupSize, height_ / workGroupSize, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	glUniform2f(2, float(width_) / 4.0f, float(height_) / 2.5f);
	glUniform1f(3, 80.0f);

	glUniform1ui(0, 1);
	glDispatchCompute(width_ / workGroupSize, height_ / workGroupSize, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	glUniform2f(2, float(width_) / 1.333f, float(height_) / 1.6666f);
	glUniform1f(3, 80.0f);

	glUniform1ui(0, 1);
	glDispatchCompute(width_ / workGroupSize, height_ / workGroupSize, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	glUniform1ui(0, 2);
	glDispatchCompute(width_ / workGroupSize, height_ / workGroupSize, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

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

	/*container::Grid<float> heightMap(width_, height_);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, finalBuffer);
	glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, width_ * height_ * sizeof(float), heightMap.getStart());
	GLuint textureKey;
	Generator::uploadTexture(&heightMap, width_, height_, GL_R32F, GL_RED, GL_FLOAT, textureKey, true);
	engine_->renderer_->addTextureToReals(TextureMaps::RELIEF_BASE_HEIGHT, textureKey);*/

	//glClearColor(0.5f, 0.1f, 0.0f, 1.0f);
	//glClearDepth(1.0f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*engine_->getShader(Shaders::MENU_PAPER).use();
	engine_->renderer_->bindTexture(Shaders::MENU_PAPER, "diffuse", 0, TextureMaps::RELIEF_BASE_HEIGHT);
	glm::vec2 scale = glm::vec2(float(width_), float(height_));
	glm::vec2 topLeftCorner = glm::vec2(0.0f, 0.0f);
	glUniformMatrix4fv(0, 1, GL_FALSE, &finalMatrix[0][0]);
	glUniform2f(1, scale.x, scale.y);
	glUniform2f(2, topLeftCorner.x, topLeftCorner.y);
	glUniform1f(3, 0.5f);
	glUniform1f(4, 1.0f);
	geometry::Geometry::models_[Models::MENU]->Render();
	engine_->getShader(Shaders::MENU_PAPER).unuse();*/

	//SDL_GL_SwapWindow(engine_->getWindow());

	/*averageHeight_ = 0.65f;
	for(int x = 0; x < vertices_.getWidth(); ++x)
		for(int y = 0; y < vertices_.getHeight(); ++y) {
			vertices_(x, y)->position_.x = float(x);
			vertices_(x, y)->position_.y = float(y);
			vertices_(x, y)->position_.z = *heightMap(x, y);

			tiles_(x, y)->position_.x = float(x) + 0.5f;
			tiles_(y, y)->position_.y = float(y) + 0.5f;
			tiles_(x, y)->position_.z = *heightMap(x, y) * 0.25f + *heightMap(x + 1, y) * 0.25f + *heightMap(x, y + 1) * 0.25f + *heightMap(x + 1, y + 1) * 0.25f;

			if(tiles_(x, y)->position_.z > averageHeight_)
				tiles_(x, y)->type_ = Reliefs::LAND;
			else
				tiles_(x, y)->type_ = Reliefs::OCEAN;

			tiles_(x, y)->vertices_[0] = vertices_(x, y);
			tiles_(x, y)->vertices_[1] = vertices_(x + 1, y);
			tiles_(x, y)->vertices_[2] = vertices_(x, y + 1);
			tiles_(x, y)->vertices_[3] = vertices_(x + 1, y + 1);
		}
	computeHeights();*/
}

void ReliefGenerator::SetupBuffers(World& world, const WorldParameterSet& parameterSet)
{
	auto size = parameterSet.Size_;
	auto area = size.x * size.y;

	DataBuffer* buffer = nullptr;

	buffer = buffers.Add(Buffers::FINAL);
	if(buffer)
	{
		buffer->Generate(area * sizeof(float));
	}

	buffer = buffers.Add(Buffers::CARVE);
	if(buffer)
	{
		buffer->Generate(area * sizeof(float));
	}

	buffer = buffers.Add(Buffers::GRADIENT);
	if(buffer)
	{
		buffer->Generate(area * sizeof(float) * 2);
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
	Perlin::Generate(size, Range(0.0f, 1.0f), 2.0f, 2.0f, 0.5f, 2.0f);
}
