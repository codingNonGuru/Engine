#include <stdlib.h>
#include "GL/glew.h"

#include "Sprite.hpp"
#include "Texture.hpp"
#include "Camera.hpp"
#include "Element.hpp"
#include "Model.hpp"
#include "Shader.hpp"
#include "Transform.hpp"

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

	shader_->SetConstant(camera->GetMatrix(), "viewMatrix");

	shader_->SetConstant(parent_->GetGlobalPosition(), "spritePosition");

	Scale2 scale;
	if(texture_)
	{
		auto textureSize = texture_->GetSize();
		scale = Scale2(textureSize.x, textureSize.y);
	}
	else
	{
		auto parentSize = parent_->GetSize();
		scale = Scale2(parentSize.x, parentSize.y);
	}
	scale *= parent_->GetTransform()->GetScale();
	shader_->SetConstant(scale, "spriteSize");

	Opacity opacity = opacity_ * parent_->GetOpacity();
	shader_->SetConstant(opacity, "opacity");

	auto drawOrder = (float)parent_->GetDrawOrder() * 0.1f;
	shader_->SetConstant(drawOrder, "depth");

	if(texture_)
	{
		//shader_->BindTexture(texture_, "diffuse");
		texture_->Bind(0, shader_, "diffuse");
	}

	glDrawArrays(GL_TRIANGLES, 0, 6);

	shader_->Unbind();

	if(texture_)
	{
		texture_->Unbind();
	}
}

float & Sprite::GetOpacity()
{
	return opacity_;
}
