#pragma once

#include "Render/Filter.hpp"

class Camera;

class SaturateFilter : public Filter
{
protected:
	virtual void HandleUpdate(Camera*) override;
};