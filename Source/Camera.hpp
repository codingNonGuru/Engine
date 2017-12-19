#pragma once

#include "Conventions.hpp"
#include "Types.hpp"

class World;
class Screen;

class Camera
{
	CameraTypes type_;

	float viewDistance_;

	glm::vec3 from_;

	glm::vec3 to_;

	glm::vec3 up_;

	glm::vec3 viewDirection_;

	glm::vec3 pivot_;

	float zenith_;

	float azimuth_;

	float scrollImpulse_;

	float spinImpulse_;

	glm::vec3 driftImpulse_;

	glm::mat4x4 finalMatrix_;

	Screen* screen_;

public:
	Camera();

	Camera(Screen*, Position3, float, float, float);

	Camera(Screen*, Position3, Position3);

	Camera(Screen*);

	Matrix & GetMatrix();

	void ComputeMatrix();

	void SetTarget(glm::vec3);

	void Drag(glm::vec3);

	void Zoom(float impulse);

	void Spin(float impulse);

	void SetTargetAsPivot();

	Position3 GetPosition() const {return from_;}

	void Update();
};
