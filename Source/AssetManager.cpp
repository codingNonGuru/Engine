#include <stdio.h>

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

char* code = new char[1024 * 1024];

void AssetManager::LoadAssets()
{
	FileManager::Initialize();

	files_ = FileManager::GetFilesInFolder(".", true);

	ShaderManager::LoadShaders();

	MeshManager::LoadMeshes();

	//CountScripts();

	OnAssetsLoaded_->Invoke();
}

void AssetManager::CountScripts()
{
	Length scriptCount = 0;
	Length lineCount = 0;
	for(auto file = files_->GetStart(); file != files_->GetEnd(); ++file)
	{
		if((FindString(file->GetName(), ".hpp") || FindString(file->GetName(), ".cpp")) && !FindString(file->GetName(), "FastDelegate") && !FindString(file->GetName(), ".o") && FindString(file->GetPath(), "Source"))
		{
			FILE* fileStream = fopen(file->GetPath(), "r");

			fseek(fileStream, 0, SEEK_END);
			Length fileSize = ftell(fileStream);
			rewind(fileStream);

			std::cout<<file->GetName()<<" "<<fileSize<<"\n";

			fread(code, 1, fileSize, fileStream);
			for(auto sign = code; sign != code + fileSize; ++sign)
			{
				if(*sign == '\n')
				{
					lineCount++;
				}
			}

			fclose(fileStream);

			scriptCount++;
		}
	}
	std::cout<<"\n";
	std::cout<<scriptCount<<" Scripts found.\n";
	std::cout<<lineCount<<" Lines found.\n";
}

Array <File>* AssetManager::GetFiles()
{
	return files_;
}

Map <AttributeType, LongWord> & AssetManager::GetAttributeTypes()
{
	return attributeTypes_;
}



