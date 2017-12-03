#include <GL/glew.h>

#include "Model.hpp"
#include "Buffer.hpp"
#include "Mesh.hpp"

void Model::Initialize(Mesh* mesh, Shader* shader, Length textureCount)
{
	mesh_ = mesh;
	shader_ = shader;

	textures_.Initialize(textureCount);

	SetupBuffer();
}

void Model::AddTexture(Texture* texture, const char* name)
{
	auto texturePointer = textures_.Allocate(LongWord(name));
	*texturePointer = texture;
}

void Model::Render(Camera* camera)
{
}

void Model::SetupBuffer()
{
	buffer_ = new MasterBuffer(4);
	buffer_->Bind();

	int index = 0;
	auto meshAttributes = mesh_->GetAttributes();
	auto meshAttributeKey = meshAttributes.GetFirstKey();
	for(auto meshAttribute = meshAttributes.GetStart(); meshAttribute != meshAttributes.GetEnd(); ++meshAttribute, ++index, ++meshAttributeKey)
	{
		auto meshAttributeData = meshAttribute->GetData();
		if(meshAttributeData == nullptr)
			continue;

		auto slaveBuffer = new SlaveBuffer(GL_SHADER_STORAGE_BUFFER, meshAttributeData->GetMemoryCapacity(), meshAttributeData->GetData());
		buffer_->AddBuffer(index, slaveBuffer, *meshAttributeKey);
	}
}
