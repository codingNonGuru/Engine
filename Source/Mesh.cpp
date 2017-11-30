/*
 * Mesh.cpp
 *
 *  Created on: Sep 24, 2016
 *      Author: andrei
 */
#include <stdlib.h>
#include <stddef.h>

#include <GL/glew.h>

#include "Mesh.hpp"

Mesh::Mesh() {}

template <class DataType>
Array<DataType>* Mesh::AddVertexData(const char* name, int componentCount, bool isIndices, bool isFloating)
{
	auto positionData = vertexDatas_.Allocate(name);
	*positionData = new VertexData<DataType>();
	(*positionData)->SetComponentCount(componentCount);
	(*positionData)->IsIndices(isIndices);
	(*positionData)->IsFloating(isFloating);

	return (Array<DataType>*)((*positionData)->GetData());
}

template <class DataType>
VertexData<DataType>* Mesh::GetVertexData(const char* name)
{
	auto vertexData = vertexDatas_.Get(name);
	return *vertexData;
}

GenericVertexData* Mesh::GetVertexData(const char* name)
{
	return *vertexDatas_.Get(name);
}

template <class DataType>
Array<DataType>* Mesh::GetVertexDataArray(const char* name)
{
	auto vertexData = vertexDatas_.Get(name);
	if(!vertexData)
		return nullptr;

	return (Array<DataType>*)((*vertexData)->GetData());
}

int Mesh::GetVertexDataMemoryCapacity(const char* name)
{
	auto vertexData = vertexDatas_.Get(name);
	if(!vertexData)
		return 0;

	return (*vertexData)->GetMemoryCapacity();
}

void* Mesh::GetVertexDataStart(const char* name)
{
	auto vertexData = vertexDatas_.Get(name);
	if(!vertexData)
		return 0;

	return (*vertexData)->GetData();
}

Mesh* Mesh::GenerateQuad()
{
	auto mesh = new Mesh();

	auto positionData = mesh->AddVertexData<Position>("position", 2, false, true);

	*positionData->allocate() = glm::vec2(-0.5f, -0.5f);
	*positionData->allocate() = glm::vec2(0.5f, -0.5f);
	*positionData->allocate() = glm::vec2(0.5f, 0.5f);
	*positionData->allocate() = glm::vec2(-0.5f, 0.5f);

	/*mesh->vertexCount_ = 4;
	mesh->elementCount_ = 6;

	//mesh->vertices_ = (Vertex2*)malloc(sizeof(Vertex2) * mesh->vertexCount_);
	mesh->positions_.initialize(mesh->vertexCount_);
	mesh->coordinates_.initialize(mesh->vertexCount_);
	mesh->weights_.initialize(mesh->vertexCount_);

	mesh->indices_.initialize(mesh->elementCount_);


	for(int i = 0; i < mesh->weights_.getCapacity(); ++i)
	{
		*mesh->weights_.allocate() = 1.0f;
	}

	*mesh->indices_.allocate() = 0;
	*mesh->indices_.allocate() = 1;
	*mesh->indices_.allocate() = 2;
	*mesh->indices_.allocate() = 0;
	*mesh->indices_.allocate() = 2;
	*mesh->indices_.allocate() = 3;*/

	return mesh;
}

void Mesh::Initialize(int vertexDataCount)
{
	vertexDatas_.Initialize(vertexDataCount);
}

/*void Mesh::draw(glm::mat4 &viewMatrix, glm::mat4 &worldMatrix) {
	glBindVertexArray(vertexArrayObjectIndex_);
	glUniformMatrix4fv(0, 1, GL_FALSE, &viewMatrix[0][0]);
	glUniformMatrix4fv(1, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, elementCount_, GL_UNSIGNED_INT, (GLvoid*)0);
	glBindVertexArray(0);
}*/

template <class DataType>
DataType Mesh::GetElement(const char* vertexDataName, int index)
{
	auto vertexData = vertexDatas_.Get(vertexDataName);
	if(!vertexData)
		return DataType();

	auto data = (Array<DataType>*)((*vertexData)->GetData());
	if(!data)
		return DataType();

	return data.get(index);
}

Mesh::~Mesh() {}

void MeshAtlas::Initialize(const char* name, int vertexDataCount)
{
	auto mesh = meshes_.Get(name);
	if(!mesh)
		return;

	mesh->Initialize(vertexDataCount);
}

