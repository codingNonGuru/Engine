#include <stdlib.h>
#include "GL/glew.h"

#include "Sprite.hpp"
#include "Texture.hpp"
#include "Camera.hpp"
#include "Element.hpp"
#include "Model.hpp"
#include "Shader.hpp"

Sprite::Sprite()
{
	isActive_ = true;

	parent_ = nullptr;
}

Sprite::Sprite(Texture* texture, Shader* shader)
{
	Initialize(texture, shader);
}

void Sprite::Initialize(Texture* texture, Shader* shader)
{
	isActive_ = true;

	texture_ = texture;

	shader_ = shader;

	parent_ = nullptr;

	opacity_ = 1.0f;
}

void Sprite::Draw(Camera* camera)
{
	shader_->Bind();

	Matrix& matrix = camera->GetMatrix();
	shader_->SetConstant(&matrix, "viewMatrix");

	Position3 position = parent_->GetGlobalPosition();
	shader_->SetConstant(&position, "spritePosition");

	Scale2 scale;
	if(texture_)
	{

	}
	else
	{
		auto parentSize = parent_->GetSize();
		scale = Scale2(parentSize.x, parentSize.y);
	}
	shader_->SetConstant(&scale, "spriteSize");

	Opacity opacity = opacity_ * parent_->GetOpacity();
	shader_->SetConstant(&opacity, "opacity");

	glDrawArrays(GL_TRIANGLES, 0, 6);

	shader_->Unbind();
}

float & Sprite::GetOpacity()
{
	return opacity_;
}
