#include "AssetManager.hpp"

#include "FileManager.hpp"
#include "Delegate.hpp"
#include "File.hpp"
#include "ShaderManager.hpp"
#include "MeshManager.hpp"

Array <File>* AssetManager::files_ = nullptr;

Map <AttributeType, LongWord> AssetManager::attributeTypes_ = Map <AttributeType, LongWord> ();

Delegate* AssetManager::OnAssetsLoaded_ = new Delegate();

void AssetManager::Initialize()
{
	attributeTypes_.Initialize(16);

	*attributeTypes_.Allocate(LongWord("float")) = AttributeType(LongWord("float"), 4, 1, AttributeElementTypes::FLOAT_1);
	*attributeTypes_.Allocate(LongWord("vec2")) = AttributeType(LongWord("vec2"), 8, 2, AttributeElementTypes::FLOAT_2);
	*attributeTypes_.Allocate(LongWord("vec3")) = AttributeType(LongWord("vec3"), 12, 3, AttributeElementTypes::FLOAT_3);
	*attributeTypes_.Allocate(LongWord("vec4")) = AttributeType(LongWord("vec4"), 16, 4, AttributeElementTypes::FLOAT_4);

	*attributeTypes_.Allocate(LongWord("int")) = AttributeType(LongWord("int"), 4, 1, AttributeElementTypes::INTEGER_1);
	*attributeTypes_.Allocate(LongWord("ivec2")) = AttributeType(LongWord("ivec2"), 8, 2, AttributeElementTypes::INTEGER_2);
	*attributeTypes_.Allocate(LongWord("ivec3")) = AttributeType(LongWord("ivec3"), 12, 3, AttributeElementTypes::INTEGER_3);
	*attributeTypes_.Allocate(LongWord("ivec4")) = AttributeType(LongWord("ivec4"), 16, 4, AttributeElementTypes::INTEGER_4);

	*attributeTypes_.Allocate(LongWord("uint")) = AttributeType(LongWord("uint"), 4, 1, AttributeElementTypes::UINTEGER_1);
	*attributeTypes_.Allocate(LongWord("uvec2")) = AttributeType(LongWord("uvec2"), 8, 2, AttributeElementTypes::UINTEGER_2);
	*attributeTypes_.Allocate(LongWord("uvec3")) = AttributeType(LongWord("uvec3"), 12, 3, AttributeElementTypes::UINTEGER_3);
	*attributeTypes_.Allocate(LongWord("uvec4")) = AttributeType(LongWord("uvec4"), 16, 4, AttributeElementTypes::UINTEGER_4);

	*attributeTypes_.Allocate(LongWord("mat3")) = AttributeType(LongWord("mat3"), 36, 9, AttributeElementTypes::MATRIX_3);
	*attributeTypes_.Allocate(LongWord("mat4")) = AttributeType(LongWord("mat4"), 64, 16, AttributeElementTypes::MATRIX_4);
}

void AssetManager::LoadAssets()
{
	FileManager::Initialize();

	files_ = FileManager::GetFilesInFolder(".", true);

	ShaderManager::LoadShaders();

	MeshManager::LoadMeshes();

	OnAssetsLoaded_->Invoke();
}

Array <File>* AssetManager::GetFiles()
{
	return files_;
}

Map <AttributeType, LongWord> & AssetManager::GetAttributeTypes()
{
	return attributeTypes_;
}



