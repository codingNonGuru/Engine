#include "Render/Filter.hpp"

#include "Conventions.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Camera.hpp"

Filter::Filter() {}

Filter::Filter(Shader* shader, Texture* screenTexture)
{
	Initialize(shader, screenTexture);
}

void Filter::Initialize(Shader* shader, Texture* screenTexture)
{
	shader_ = shader;

	screenTexture_ = screenTexture;
}

void Filter::Apply(Camera* camera)
{
	if(!screenTexture_)
		return;

	screenTexture_->DownloadBackBuffer();

	shader_->Bind();

	Matrix& matrix = camera->GetMatrix();
	shader_->SetConstant(matrix, "viewMatrix");

	auto textureSize = screenTexture_->GetSize();
	Scale2 scale = Scale2(textureSize.x, textureSize.y);
	shader_->SetConstant(scale, "screenSize");

	screenTexture_->Bind(0, shader_, "diffuse");

	glDrawArrays(GL_TRIANGLES, 0, 6);

	shader_->Unbind();

	screenTexture_->Unbind();
}
