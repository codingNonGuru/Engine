#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

#include <GL/glew.h>

#include "Mesh.hpp"

#include "AssetManager.hpp"
#include "File.hpp"

Mesh::Mesh() {}

Mesh* Mesh::GenerateQuad()
{
	auto mesh = new Mesh();

	return mesh;
}

void Mesh::Initialize(int vertexDataCount)
{
	attributes_.Initialize(vertexDataCount);
}

struct MeshAttributeHeader
{
	char Name_[32];

	Length ElementCount_;

	Length ElementSize_;

	char Type_[8];
};

void* attributeData = malloc(16 * MEGABYTE);

#define RED_CODE "\033[1;31m"
#define ORANGE_CODE "\033[0;91m"
#define BOLD_WHITE_CODE "\033[1;97m"
#define BLANK_CODE "\033[0m"

void Mesh::Initialize(File* file)
{
	FILE* fileStream = fopen(file->GetPath(), "rb");
	if(fileStream == nullptr)
		return;

	Length attributeCount;
	fread(&attributeCount, 1, sizeof(attributeCount), fileStream);

	std::cout << "> Mesh name: " << RED_CODE << file->GetName() << BLANK_CODE << "\n";
	std::cout << "> Attribute count: " << BOLD_WHITE_CODE << attributeCount << BLANK_CODE << "\n";

	attributes_.Initialize(attributeCount);

	MeshAttributeHeader *headers = new MeshAttributeHeader[attributeCount];
	fread(headers, 1, sizeof(MeshAttributeHeader) * attributeCount, fileStream);

	for(auto header = headers; header != headers + attributeCount; ++header)
	{
		std::cout << "---> Mesh Attribute name: " << ORANGE_CODE << header->Name_ << BLANK_CODE << "\n";
		std::cout << "---> Mesh Attribute element count: " << BOLD_WHITE_CODE << header->ElementCount_ << BLANK_CODE << "\n";
		std::cout << "---> Mesh Attribute element size: " << BOLD_WHITE_CODE << header->ElementSize_ << BLANK_CODE << "\n";
		std::cout << "---> Mesh Attribute type: " << BOLD_WHITE_CODE << header->Type_ << BLANK_CODE << "\n";
		std::cout << "\n";

		auto attribute = attributes_.Add(header->Name_);
		if(attribute == nullptr)
			continue;

		Length attributeMemorySize = header->ElementCount_ * header->ElementSize_;
		fread(attributeData, 1, attributeMemorySize, fileStream);

		if(CompareStrings(header->Type_, "vec3") == 0)
		{
			attribute->GetData() = new Array <Float3> (header->ElementCount_);
		}
		else if(CompareStrings(header->Type_, "vec2") == 0)
		{
			attribute->GetData() = new Array <Float2> (header->ElementCount_);
		}
		else if(CompareStrings(header->Type_, "float") == 0)
		{
			attribute->GetData() = new Array <Float> (header->ElementCount_);
		}
		else if(CompareStrings(header->Type_, "uint") == 0)
		{
			attribute->GetData() = new Array <Index> (header->ElementCount_);
		}

		CopyMemory(attribute->GetData()->GetData(), attributeData, attributeMemorySize);
		auto attributeType = AssetManager::GetAttributeTypes().Get(header->Type_);
		attribute->SetType(attributeType);
		attribute->SetSize(header->ElementCount_);
	}
	std::cout<<"\n";

	indexCount_ = attributes_.Get("index")->GetSize();
	vertexCount_ = attributes_.Get("position")->GetSize();

	fclose(fileStream);
	delete [] headers;
}

MeshAttribute* Mesh::GetAttribute(const char* identifier)
{
	return attributes_.Get(identifier);
}

void* Mesh::GetAttributeData(const char* identifier)
{
	auto attribute = attributes_.Get(identifier);
	if(attribute == nullptr)
		return nullptr;

	return attribute->GetData()->GetData();
}

Length Mesh::GetAttributeMemoryCapacity(const char* identifier)
{
	auto attribute = attributes_.Get(identifier);
	if(attribute == nullptr)
		return 0;

	return attribute->GetData()->GetMemoryCapacity();
}

Mesh::~Mesh() {}

