/*
 * Transform.cpp
 *
 *  Created on: Sep 25, 2016
 *      Author: andrei
 */

#include <math.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Transform.hpp"

Transform::Transform(Position3 position, Rotation rotation, Scale scale)
{
	position_ = position;
	rotation_ = rotation;
	scale_ = scale;
}

void Transform::Initialize(Position3 position, Rotation rotation, Scale scale)
{
	position_ = position;
	rotation_ = rotation;
	scale_ = scale;
}

Direction3 Transform::GetForward()
{
	return Direction3(cos(rotation_), sin(rotation_), 0.0f);
}

Matrix Transform::GetMatrix()
{
	Matrix worldMatrix =
		glm::translate(Matrix(1.0f), position_) *
		glm::rotate(Matrix(1.0f), rotation_, glm::vec3(0.0f, 0.0f, 1.0f)) *
		glm::scale(Matrix(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

	return worldMatrix;
}

Matrix Transform::GetPositionMatrix()
{
	Matrix worldMatrix =
		glm::translate(Matrix(1.0f), position_) *
		glm::scale(Matrix(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

	return worldMatrix;
}

/*Transform* Transform::Allocate(Scene* scene, Position position, Rotation rotation, Scale scale)
{
	auto transform = scene->transforms_.allocate();
	transform->Initialize(position, rotation, scale);
	return transform;
}

Transform* Transform::Allocate(Scene* scene, Transform* otherTransform)
{
	auto transform = scene->transforms_.allocate();
	*transform = *otherTransform;
	return transform;
}*/

Transform::~Transform() {
	// TODO Auto-generated destructor stub
}

