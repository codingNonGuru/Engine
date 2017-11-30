#pragma once

#include <glm.hpp>

typedef glm::vec2 Position;
typedef float Rotation;
typedef glm::vec2 Direction;
typedef float Scale;
typedef glm::vec2 Size;
typedef glm::mat4 Matrix;

typedef unsigned int Index;

#include "Memory.hpp"

template <class Type>
using Array = container::Array<Type>;
template <class Type>
using Pool = container::DynamicPool<Type>;
template <class Type>
using Map = container::StaticMap<Type>;

enum class CameraTypes {ORTHO, PERSPECTIVE};

#include "FastDelegate.hpp"

typedef fastdelegate::FastDelegate0<> Callback;

#include <stdio.h>

void* CopyMemory(void *destination, const void *source, size_t num) {return memcpy(destination, source, num);}
