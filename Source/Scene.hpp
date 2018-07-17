#pragma once

#include "Object.hpp"

class Camera;

class Scene : public Object
{
protected:
	Camera* camera_;

public:
	virtual void Initialize();

	virtual void Render();
};
