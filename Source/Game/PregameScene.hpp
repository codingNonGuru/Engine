#pragma once

#include "Scene.hpp"

class PregameScene : public Scene
{
	virtual void Update() override;

public:
	PregameScene();

	virtual void Render() override;
};
