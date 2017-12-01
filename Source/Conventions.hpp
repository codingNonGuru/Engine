#pragma once

#include <glm/glm.hpp>

typedef glm::vec2 Position;
typedef float Rotation;
typedef glm::vec2 Direction;
typedef float Scale;
typedef glm::vec2 Size;
typedef glm::mat4 Matrix;

typedef unsigned int Index;
typedef unsigned int Length;

#include "Memory.hpp"

template <class Type>
using Array = container::Array<Type>;
template <class Type>
using Pool = container::DynamicPool<Type>;
template <class ValueType, class KeyType = ShortWord>
using Map = container::StaticMap<ValueType, KeyType>;

enum class CameraTypes {ORTHO, PERSPECTIVE};

#include "FastDelegate.hpp"

typedef fastdelegate::FastDelegate0<> Callback;

void* CopyMemory(void *, const void *, size_t);

int CompareMemory(const void *, const void *, size_t);

Length GetStringLength(const char *);
