#include <stdlib.h>
#include <gtx/transform.hpp>

#include "Camera.hpp"
#include "Screen.hpp"

Camera::Camera() {}

Camera::Camera(glm::vec3 to, float viewDistance)
{
	to_ = to;
	viewDistance_ = viewDistance;
	viewAngle_ = 0.3f;
	viewDirection_ = glm::vec3(0.0f, cos(viewAngle_), sin(viewAngle_));
	viewDirection_ = glm::normalize(viewDirection_);
	from_ = to_ + (viewDirection_ * viewDistance_);
	up_ = glm::vec3(0.0f, -sin(viewAngle_), cos(viewAngle_));
	scrollImpulse_ = 0.0f;
	spinImpulse_ = 0.0f;
	driftImpulse_ = glm::vec3(0.0f, 0.0f, 0.0f);

	azimuth_ = 1.57079f;
	zenith_ = 0.57f;
}

Camera::Camera(CameraTypes type)
{
	type_ = type;
}

glm::mat4x4 Camera::getViewMatrix()
{
	return glm::lookAt<float>(from_, to_, up_);
}

glm::mat4x4 Camera::GetMatrix()
{
	return finalMatrix_;
}

void Camera::ComputeMatrix(Screen* screen)
{
	if(type_ == CameraTypes::ORTHO)
	{
		glm::vec3 screenCenter(-screen->getWidthFloating() * 0.5f, -screen->getHeightFloating() * 0.5f, 0.0f);
		glm::mat4 projectionMatrix = glm::ortho<float> (0.0f, screen->getWidthFloating(), screen->getHeightFloating(), 0.0f, 0.1f, 10.0f);
		glm::mat4 viewMatrix = glm::lookAt<float> (screenCenter + glm::vec3(0.0f, 0.0f, 5.0f), screenCenter, glm::vec3(0.0f, 1.0f, 0.0f));
		finalMatrix_ = projectionMatrix * viewMatrix;
	}
}

void Camera::SetTarget(glm::vec3 target)
{
	to_ = target;
	azimuth_ = 1.57079f;
	/*if(to_.y < 0) to_.y = 0;
	if(to_.y > world.height_) to_.y = world.height_;
	if(to_.x < 0) to_.x = 0;
	if(to_.x > world.width_) to_.x = world.width_;*/
}

void Camera::Drag(glm::vec3 targetDelta)
{
	driftImpulse_ += targetDelta;

	to_ += driftImpulse_;
	/*if(to_.y < 0.0f) to_.y = 0.0f;
	if(to_.y > world.height_) to_.y = world.height_ - 0.001f;
	if(to_.x < 0.0f) to_.x = 0.0f;
	if(to_.x > world.width_) to_.x = world.width_ - 0.001f;
	Tile& G = *const_cast<World&>(world).tiles_(to_.x, to_.y);
	if(1 - (to_.x - (int)to_.x) > to_.y - (int)to_.y) 
		to_.z = utility::Utility::getHeightFromPlaneNormal(G.getNormal(0), G.vertices_[0]->position_, to_.x, to_.y);
	else
		to_.z = utility::Utility::getHeightFromPlaneNormal(G.getNormal(1), G.vertices_[3]->position_, to_.x, to_.y);*/
}

void Camera::Zoom(float impulse)
{
	scrollImpulse_ -= impulse;

	viewDistance_ += scrollImpulse_;
	if(viewDistance_ < 1.0f)
		viewDistance_ = 1.0f;
	else if(viewDistance_ > 600.0f)
		viewDistance_ = 600.0f;
}

void Camera::Spin(float impulse)
{
	spinImpulse_ -= impulse * 0.0001f;

	azimuth_ += spinImpulse_;
	if(azimuth_ > 6.2831f)
		azimuth_ -= 6.2831f;
	if(azimuth_ < 0.0f)
		azimuth_ += 6.2831f;
}

void Camera::Update()
{
	viewDirection_ = glm::vec3(sin(zenith_) * cos(azimuth_), sin(zenith_) * sin(azimuth_), cos(zenith_));
	from_ = to_ + (viewDirection_ * viewDistance_);

	glm::vec2 horizontal(viewDirection_.x, viewDirection_.y);
	float vertical = glm::length(horizontal);
	horizontal = -glm::normalize(horizontal) * viewDirection_.z;
	up_ = glm::vec3(horizontal.x, horizontal.y, vertical);
	scrollImpulse_ *= 0.95f;
	spinImpulse_ *= 0.95f;
	driftImpulse_ *= 0.95f;
}
