#pragma once

#define KILOBYTE 1024

#define MEGABYTE 1048576

#include <glm/glm.hpp>

typedef glm::vec2 Float2;
typedef glm::vec3 Float3;
typedef glm::vec4 Float4;

typedef glm::vec2 Position2;
typedef glm::vec3 Position3;
typedef float Rotation;
typedef glm::vec2 Direction2;
typedef glm::vec3 Direction3;
typedef float Scale;
typedef glm::vec2 Scale2;
typedef glm::ivec2 Size;
typedef glm::mat4 Matrix;
typedef glm::vec4 Color;
typedef int DrawOrder;
typedef float Opacity;

typedef unsigned int Index;
typedef unsigned int Length;

#include "Memory.hpp"

typedef container::String<32> FileName;
typedef container::String<256> FilePath;

typedef container::Container Container;
template <class Type>
using Array = container::Array<Type>;
template <class Type>
using Pool = container::DynamicPool<Type>;
template <class ValueType, class KeyType = ShortWord>
using Map = container::StaticMap<ValueType, KeyType>;

#include "FastDelegate.hpp"

typedef fastdelegate::FastDelegate0<> Callback;

void* AllocateMemory(unsigned int);

void* CopyMemory(void *, const void *, size_t);

int CompareMemory(const void *, const void *, size_t);

int CompareStrings(const char *, const char *);

Length GetStringLength(const char *);

const char* FindString(const char*, const char*);

const char* FindLastOccurrence(const char*, char);

#define DEBUG_OPENGL DebugGl(__FILE__, __LINE__);

void DebugGl(const char*, int);
