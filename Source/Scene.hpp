#pragma once

#include "Object.hpp"

class Camera;

class Scene : public Object
{
	Camera* camera_;

public:
	virtual void Initialize();

	virtual void Render();
};
