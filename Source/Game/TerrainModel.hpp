#pragma once

#include "Model.hpp"

class Camera;
class Light;

class TerrainModel : public Model
{
public:
	TerrainModel();

	virtual void Initialize() override;

	virtual void Render(Camera*, Light*) override;
};
