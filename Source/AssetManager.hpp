#pragma once

#include "Conventions.hpp"
#include "Attribute.hpp"

class Delegate;
class File;

class AssetManager
{
private:
	static Array <File> *files_;

	static Map <AttributeType, LongWord> attributeTypes_;

	static void LoadTextures();

	static void LoadMeshes();

	static void CountScripts();

public:
	static Delegate* OnAssetsLoaded_;

	static Array <File> *GetFiles();

	static Map <AttributeType, LongWord> & GetAttributeTypes();

	static void Initialize();

	static void LoadAssets();
};
