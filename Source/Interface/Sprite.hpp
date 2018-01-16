#pragma once

#include <glm/glm.hpp>

#include "Conventions.hpp"

typedef unsigned int GLuint;
typedef unsigned int GLenum;

class Texture;
class Element;
class Shader;
class Camera;
class Model;

class Sprite
{
private:
	bool isActive_;

	float opacity_;

	float depth_;

	Texture* texture_;

	Shader* shader_;

	Element* parent_;

	Color color_;

public:
	Sprite();

	Sprite(Texture*, Shader*);

	void Initialize(Texture*, Shader*);

	void Draw(Camera*);

	void SetParent(Element* parent) {parent_ = parent;}

	void SetColor(Color color) {color_ = color;}

	float & GetOpacity();
};
