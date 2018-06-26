#include "Texture.hpp"
#include "TextureManager.hpp"

#include "Game/WorldGenerator.hpp"
#include "Game/WorldParameterSet.hpp"
#include "Game/ReliefGenerator.hpp"

Map <Texture*, WorldSizeOptions> WorldGenerator::reliefTextures_ = Map <Texture*, WorldSizeOptions>((int)WorldSizeOptions::COUNT);

WorldParameterSet* WorldGenerator::parameterSet_ = nullptr;

Delegate WorldGenerator::OnWorldGenerated_ = Delegate();

void WorldGenerator::Generate(World& world, const WorldParameterSet& parameterSet)
{
	parameterSet_ = &parameterSet;

	Size size;
	switch(parameterSet.SizeOption_)
	{
	case WorldSizeOptions::TINY:
		size = Size(256, 256);
		break;
	case WorldSizeOptions::SMALL:
		size = Size(512, 512);
		break;
	case WorldSizeOptions::MEDIUM:
		size = Size(768, 768);
		break;
	case WorldSizeOptions::LARGE:
		size = Size(1024, 1024);
		break;
	case WorldSizeOptions::IMMENSE:
		size = Size(1536, 1536);
		break;
	}

	ReliefGenerator::Generate(world, size);

	OnWorldGenerated_.Invoke();
}

Texture* WorldGenerator::GetReliefPreview()
{
	auto texturePointer = reliefTextures_.Get(parameterSet_->SizeOption_);
	if(texturePointer != nullptr)
		return *texturePointer;

	Texture* previewTexture = nullptr;

	switch(parameterSet_->SizeOption_)
	{
	case WorldSizeOptions::TINY:
		previewTexture = new Texture(Size(256, 256), TextureFormats::FOUR_BYTE, nullptr);
		break;
	case WorldSizeOptions::SMALL:
		previewTexture = new Texture(Size(512, 512), TextureFormats::FOUR_BYTE, nullptr);
		break;
	case WorldSizeOptions::MEDIUM:
		previewTexture = new Texture(Size(768, 768), TextureFormats::FOUR_BYTE, nullptr);
		break;
	case WorldSizeOptions::LARGE:
		previewTexture = new Texture(Size(1024, 1024), TextureFormats::FOUR_BYTE, nullptr);
		break;
	case WorldSizeOptions::IMMENSE:
		previewTexture = new Texture(Size(1536, 1536), TextureFormats::FOUR_BYTE, nullptr);
		break;
	}

	TextureManager::AddTexture(previewTexture, "WorldPreview");
	*reliefTextures_.Add(parameterSet_->SizeOption_) = previewTexture;

	return previewTexture;
}
