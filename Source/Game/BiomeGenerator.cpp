#include "ImageProcessor.hpp"
#include "Conventions.hpp"
#include "DataBuffer.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "ShaderManager.hpp"
#include "Utility/Perlin.hpp"

#include "BiomeGenerator.hpp"
#include "Game/World.hpp"
#include "Game/ReliefGenerator.hpp"
#include "Game/WorldGenerator.hpp"
#include "Game/Tile.hpp"

DataBuffer* reliefBuffer = nullptr;

void BiomeGenerator::Generate(World& world)
{
	auto size = world.GetSize();

	auto& tiles = world.GetTiles();
	Grid <Float> reliefs(size.x, size.y);
	for(int x = 0; x < size.x; ++x)
	{
		for(int y = 0; y < size.y; ++y)
		{
			auto tile = tiles(x, y);
			if(tiles(x, y)->GetRelief() == ReliefTypes::OCEAN)
			{
				*reliefs(x, y) = 1.0f;
			}
			else
			{
				*reliefs(x, y) = 0.0f;
			}
		}
	}

	if(reliefBuffer == nullptr)
	{
		reliefBuffer = new DataBuffer(reliefs.GetMemoryCapacity(), reliefs.GetStart());
	}
	else
	{
		reliefBuffer->UploadData(reliefs.GetStart(), reliefs.GetMemoryCapacity());
	}

	auto output = ImageProcessor::Blur(reliefBuffer, size, 128);

	auto variation = Perlin::Generate(size, Range(0.0f, 1.0f), 2.0f, 2.0f, 0.5f, 2.0f);

	auto shader = ShaderManager::GetShader("GenerateBiome");
	if(shader == nullptr)
		return;

	shader->Bind();

	output->Bind(0);
	variation->Bind(1);

	shader->SetConstant(size, "size");

	shader->SetConstant(0, "stage");
	shader->DispatchCompute(size / 4);

	shader->SetConstant(1, "stage");
	shader->DispatchCompute(size / 4);

	shader->Unbind();

	Grid <Float> biomes(&reliefs);
	output->Download(&biomes);

	Grid <Byte4> textureData(size.x, size.y);
	for(int x = 0; x < size.x; ++x)
	{
		for(int y = 0; y < size.y; ++y)
		{
			Byte intensity;
			if(*biomes(x, y) > 0.75f)
				intensity = 64;
			else if(*biomes(x, y) > 0.5f)
				intensity = 128;
			else if(*biomes(x, y) > 0.25f)
				intensity = 192;
			else
				intensity = 255;

			*textureData(x, y) = Byte4(intensity, 0, 0, tiles(x, y)->GetRelief() == ReliefTypes::OCEAN ? 0 : 255);
			//*textureData(x, y) = Byte4(128, 0, 0, 255.0f * *biomes(x, y));
		}
	}

	auto texture = WorldGenerator::GetBiomePreview();
	texture->Upload(textureData.GetStart());
}

void BiomeGenerator::SetupBuffers(World& world)
{

}
