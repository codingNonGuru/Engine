#include <GL/glew.h>

#include "Model.hpp"
#include "Buffer.hpp"
#include "Mesh.hpp"

void Model::Initialize(Mesh* mesh)
{
	mesh_ = mesh;

	buffer_ = new MasterBuffer(4);
	buffer_->Bind();

	int index = 0;
	auto vertexDatas = mesh->GetVertexDatas();
	auto vertexDataKey = vertexDatas.GetFirstKey();
	for(auto vertexDataIterator = vertexDatas.GetStart(); vertexDataIterator != vertexDatas.GetEnd(); ++vertexDataIterator)
	{
		if(vertexDataIterator == nullptr)
			continue;

		auto vertexData = *vertexDataIterator;
		if(vertexData == nullptr)
			continue;

		auto slaveBuffer = new SlaveBuffer(
			vertexData->IsIndices() ? GL_ELEMENT_ARRAY_BUFFER : GL_ARRAY_BUFFER,
			vertexData->GetMemoryCapacity(),
			vertexData->GetData()
		);
		if(vertexData->IsIndices())
		{
			buffer_->AddElementBuffer(slaveBuffer);
		}
		else
		{
			buffer_->AddBuffer(index, slaveBuffer, *vertexDataKey, vertexData->GetComponentCount(), vertexData->IsFloating());
		}
		index++;
		vertexDataKey++;
	}
}

void Model::Render(Camera* camera)
{
}
