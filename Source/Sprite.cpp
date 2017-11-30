#include <stdlib.h>
#include "GL/glew.h"

#include "Sprite.hpp"
#include "Texture.hpp"
#include "Camera.hpp"
#include "Element.hpp"
#include "Model.hpp"

Sprite::Sprite()
{
	isActive_ = true;
	parent_ = nullptr;
}

Sprite::Sprite(Texture* texture, Shader* shader, Model* model, Element* parent)
{
	Initialize(texture, shader, model, parent);
}

void Sprite::Initialize(Texture* texture, Shader* shader, Model* model, Element* parent)
{
	isActive_ = true;

	texture_ = texture;
	shader_ = shader;
	model_ = model;

	parent_ = parent;
}

void Sprite::Draw(Camera* camera)
{
	if(isActive_ == false)
		return;

	if(!shader_ || !texture_ || !model_)
		return;

	texture_->Bind(0, shader_, "diffuse");

	glm::vec2 position = parent_->GetPosition();

	glm::vec2 size = texture_->GetSize();
	glm::vec2 topLeftCorner = position - size * 0.5f;
	glm::mat4x4 matrix = camera->GetMatrix();
	glUniformMatrix4fv(0, 1, GL_FALSE, &matrix[0][0]);
	glUniform2f(1, size.x, size.y);
	glUniform2f(2, topLeftCorner.x, topLeftCorner.y);
	glUniform1f(3, 0.5f);
	glUniform1f(4, opacity_);
	model_->Render(camera);
}
