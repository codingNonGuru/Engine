#include "Texture.hpp"
#include "TextureManager.hpp"

#include "Game/WorldGenerator.hpp"
#include "Game/WorldParameterSet.hpp"
#include "Game/ReliefGenerator.hpp"
#include "Game/BiomeGenerator.hpp"
#include "Game/CultureGenerator.hpp"
#include "Game/World.hpp"

Map <Texture*, WorldSizeOptions> WorldGenerator::reliefTextures_ = Map <Texture*, WorldSizeOptions>((int)WorldSizeOptions::COUNT);

Map <Texture*, WorldSizeOptions> WorldGenerator::biomeTextures_ = Map <Texture*, WorldSizeOptions>((int)WorldSizeOptions::COUNT);

Map <Texture*, WorldSizeOptions> WorldGenerator::cultureTextures_ = Map <Texture*, WorldSizeOptions>((int)WorldSizeOptions::COUNT);

const WorldParameterSet* WorldGenerator::parameterSet_ = nullptr;

Delegate WorldGenerator::OnWorldGenerated_ = Delegate();

Delegate WorldGenerator::OnReliefGenerated_ = Delegate();

Delegate WorldGenerator::OnBiomeGenerated_ = Delegate();

void WorldGenerator::Generate(World& world, const WorldParameterSet& parameterSet)
{
	parameterSet_ = &parameterSet;

	auto size = GetSize(parameterSet_->SizeOption_);
	world.SetSize(size);

	ReliefGenerator::Generate(world);

	OnReliefGenerated_.Invoke();

	BiomeGenerator::Generate(world);

	OnBiomeGenerated_.Invoke();

	CultureGenerator::Generate(world);

	OnWorldGenerated_.Invoke();
}

Texture* WorldGenerator::GetReliefPreview()
{
	auto texturePointer = reliefTextures_.Get(parameterSet_->SizeOption_);
	if(texturePointer != nullptr)
		return *texturePointer;

	auto size = GetSize(parameterSet_->SizeOption_);
	Texture* previewTexture = new Texture(size, TextureFormats::FOUR_BYTE, nullptr);

	TextureManager::AddTexture(previewTexture, "WorldPreview");
	*reliefTextures_.Add(parameterSet_->SizeOption_) = previewTexture;

	return previewTexture;
}

Texture* WorldGenerator::GetBiomePreview()
{
	auto texturePointer = biomeTextures_.Get(parameterSet_->SizeOption_);
	if(texturePointer != nullptr)
		return *texturePointer;

	auto size = GetSize(parameterSet_->SizeOption_);
	Texture* previewTexture = new Texture(size, TextureFormats::FOUR_BYTE, nullptr);

	TextureManager::AddTexture(previewTexture, "WorldPreview");
	*biomeTextures_.Add(parameterSet_->SizeOption_) = previewTexture;

	return previewTexture;
}

Texture* WorldGenerator::GetCulturePreview()
{
	auto texturePointer = cultureTextures_.Get(parameterSet_->SizeOption_);
	if(texturePointer != nullptr)
		return *texturePointer;

	auto size = GetSize(parameterSet_->SizeOption_);
	Texture* previewTexture = new Texture(size, TextureFormats::FOUR_BYTE, nullptr);;

	TextureManager::AddTexture(previewTexture, "WorldPreview");
	*cultureTextures_.Add(parameterSet_->SizeOption_) = previewTexture;

	return previewTexture;
}

Size WorldGenerator::GetSize(WorldSizeOptions sizeOption)
{
	switch(sizeOption)
	{
	case WorldSizeOptions::TINY:
		return Size(256, 256);
	case WorldSizeOptions::SMALL:
		return Size(512, 512);
	case WorldSizeOptions::MEDIUM:
		return Size(768, 768);
	case WorldSizeOptions::LARGE:
		return Size(1024, 1024);
	case WorldSizeOptions::IMMENSE:
		return Size(1536, 1536);
	}

	return Size();
}
