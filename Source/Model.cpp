#include <GL/glew.h>
#include <glm/gtx/transform.hpp>

#include "Model.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "HeaderBuffer.hpp"
#include "DataBuffer.hpp"

Model::Model() {}

Model::Model(Mesh* mesh, Shader* shader)
{
	Initialize(mesh, shader);
}

void Model::Initialize(Mesh* mesh, Shader* shader)
{
	mesh_ = mesh;
	shader_ = shader;

	textures_.Initialize(shader_->GetTextureCount());

	SetupBuffer();
}

void Model::AddTexture(Texture* texture, const char* name)
{
	auto texturePointer = textures_.Allocate(LongWord(name));
	*texturePointer = texture;
}

void Model::Render(Camera* camera)
{
	shader_->Bind();

	Matrix& matrix = camera->GetMatrix();
	shader_->SetConstant(&matrix, "viewMatrix");

	auto indexCount = mesh_->GetIndexCount();
	shader_->SetConstant(&indexCount, "indexCount");

	auto vertexCount = mesh_->GetVertexCount();
	shader_->SetConstant(&vertexCount, "vertexCount");

	auto cameraPosition = camera->GetPosition();
	shader_->SetConstant(&cameraPosition, "cameraPosition");

	buffer_->SetSlaveBindPoint("position", 0);
	buffer_->SetSlaveBindPoint("normal", 1);
	buffer_->SetSlaveBindPoint("index", 2);

	glDrawArrays(GL_TRIANGLES, 0, indexCount);

	shader_->Unbind();
}

void Model::SetupBuffer()
{
	buffer_ = new HeaderBuffer(4);
	buffer_->Bind();

	int index = 0;
	auto meshAttributes = mesh_->GetAttributes();
	auto meshAttributeKey = meshAttributes.GetFirstKey();
	for(auto meshAttribute = meshAttributes.GetStart(); meshAttribute != meshAttributes.GetEnd(); ++meshAttribute, ++index, ++meshAttributeKey)
	{
		auto meshAttributeData = meshAttribute->GetData();
		if(meshAttributeData == nullptr)
			continue;

		auto slaveBuffer = new DataBuffer(GL_SHADER_STORAGE_BUFFER, meshAttributeData->GetMemoryCapacity(), meshAttributeData->GetData());
		buffer_->AddBuffer(index, slaveBuffer, *meshAttributeKey);
	}
}
