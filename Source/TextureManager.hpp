#pragma once

#include "Conventions.hpp"

class Texture;

class TextureManager
{
private:
	static Map <Texture, LongWord> textures_;

public:
	static void LoadTextures();

	static Map <Texture, LongWord> & GetTextures();
};
