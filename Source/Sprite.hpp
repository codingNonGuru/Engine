#ifndef SOURCE_ENGINE_SPRITE_HPP_
#define SOURCE_ENGINE_SPRITE_HPP_

#include <glm.hpp>

#include "Memory.hpp"

typedef unsigned int GLuint;
typedef unsigned int GLenum;

class Texture;
class Element;
class Shader;
class Camera;
class Model;

class Sprite
{
public:
	bool isActive_;

	//Transform* transform;
	float opacity_;

	Texture* texture_;
	Shader* shader_;
	Element* parent_;
	Model* model_;

	Sprite();
	Sprite(Texture*, Shader*, Model*, Element*);
	void Initialize(Texture*, Shader*, Model*, Element*);
	void Draw(Camera*);
	void SetParent(Element* parent) {parent_ = parent;}
};

#endif
