/*
 * Transform.hpp
 *
 *  Created on: Sep 25, 2016
 *      Author: andrei
 */

#ifndef TRANSFORM_HPP_
#define TRANSFORM_HPP_

#include <glm.hpp>
#include "Conventions.hpp"

class Scene;

class Transform
{
public:
	Position3 position_;
	Rotation rotation_;
	Scale scale_;

	Direction3 GetForward();
	Transform() {}
	Transform(Position3, Rotation, Scale);
	void Initialize(Position3, Rotation, Scale);
	Matrix GetMatrix();
	Matrix GetPositionMatrix();
	//static Transform* Allocate(Scene*, Position, Rotation, Scale);
	//static Transform* Allocate(Scene*, Transform*);
	virtual ~Transform();
};

#endif /* TRANSFORM_HPP_ */
