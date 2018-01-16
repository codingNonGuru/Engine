#include "TextureManager.hpp"

#include "Texture.hpp"
#include "File.hpp"
#include "AssetManager.hpp"

#define MAXIMUM_TEXTURE_COUNT 256

Map <Texture, LongWord> TextureManager::textures_ = Map <Texture, LongWord> (MAXIMUM_TEXTURE_COUNT);

void TextureManager::LoadTextures()
{
	auto files = AssetManager::GetFiles();

	for(File* file = files->GetStart(); file != files->GetEnd(); ++file)
	{
		auto textureExtension = ".tex";

		auto extensionPosition = FindString(file->GetName(), textureExtension);
		if(extensionPosition == nullptr)
			continue;

		LongWord textureName;
		textureName.Add(file->GetName(), extensionPosition - file->GetName());

		auto texture = textures_.Add(textureName);
		if(texture == nullptr)
			continue;

		texture->Initialize(file);
	}
}

Map <Texture, LongWord> & TextureManager::GetTextures()
{
	return textures_;
}
