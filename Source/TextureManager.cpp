#include "TextureManager.hpp"

#include "Texture.hpp"
#include "File.hpp"
#include "AssetManager.hpp"

#define MAXIMUM_TEXTURE_COUNT 256

#define TEXTURE_EXTENSION_COUNT 2

Map <Texture*, LongWord> TextureManager::textures_ = Map <Texture*, LongWord> (MAXIMUM_TEXTURE_COUNT);

void TextureManager::LoadTextures()
{
	auto files = AssetManager::GetFiles();

	const char* extensions[TEXTURE_EXTENSION_COUNT] = {".tex", ".png"};
	for(File* file = files->GetStart(); file != files->GetEnd(); ++file)
	{
		for(auto extensionIterator = extensions; extensionIterator != extensions + TEXTURE_EXTENSION_COUNT; ++extensionIterator)
		{
			auto extension = *extensionIterator;
			if(extension == nullptr)
				continue;

			auto extensionPosition = FindString(file->GetName(), extension);
			if(extensionPosition == nullptr)
				continue;

			LongWord textureName;
			textureName.Add(file->GetName(), extensionPosition - file->GetName());

			auto texturePointer = textures_.Add(textureName);
			if(texturePointer == nullptr)
				continue;

			*texturePointer = new Texture(file);
		}
	}
}

Texture* TextureManager::GetTexture(const char* identifier)
{
	auto texturePointer = textures_.Get(identifier);
	if(texturePointer == nullptr)
		return nullptr;

	return *texturePointer;
}

void TextureManager::AddTexture(Texture* texture, const char* identifier)
{
	auto texturePointer = textures_.Add(identifier);
	if(texturePointer == nullptr)
		return;

	*texturePointer = texture;
}

Map <Texture*, LongWord> & TextureManager::GetTextures()
{
	return textures_;
}
