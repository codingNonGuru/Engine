#pragma once

#include "Conventions.hpp"
#include "Types.hpp"

class World;
class Screen;

class Camera
{
public:
	CameraTypes type_;

	float viewDistance_;
	glm::vec3 from_;
	glm::vec3 to_;
	glm::vec3 up_;
	glm::vec3 viewDirection_;
	float viewAngle_;
	float scrollImpulse_;
	float spinImpulse_;
	glm::vec3 driftImpulse_;
	float azimuth_;
	float zenith_;
	glm::mat4x4 finalMatrix_;

	Camera();

	Camera(Position3 to, float viewDistance);

	Camera(CameraTypes);

	glm::mat4x4 getViewMatrix();

	glm::mat4x4 GetMatrix();

	void ComputeMatrix(Screen*);

	void SetTarget(glm::vec3);

	void Drag(glm::vec3);

	void Zoom(float impulse);

	void Spin(float impulse);

	void Update();
};
