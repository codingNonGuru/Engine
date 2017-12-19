#pragma once

#include "Conventions.hpp"
#include "Types.hpp"
#include "Attribute.hpp"

class Mesh;

struct MeshAttributeType : public AttributeType
{
	MeshAttributeTypes AttributeType_;

	MeshAttributeType() {}

	MeshAttributeType(LongWord identifier, Length memorySize, Length componentCount, AttributeElementTypes elementType, MeshAttributeTypes attributeType) :
		AttributeType(identifier, memorySize, componentCount, elementType), AttributeType_(attributeType) {}
};

class MeshManager
{
private:
	static Map <Mesh, LongWord> meshes_;

public:
	static void Initialize();

	static void LoadMeshes();

	static Map <Mesh, LongWord> & GetMeshes();
};
