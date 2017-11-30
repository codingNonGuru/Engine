#pragma once

#include <glm.hpp>

#include "Conventions.hpp"

class GenericVertexData
{
	int componentCount_;

	bool isIndices_;

	bool isFloating_;
public:
	virtual void* GetData() = 0;

	virtual int GetMemoryCapacity() = 0;

	int GetComponentCount() const {return componentCount_;}

	void SetComponentCount(int componentCount) {componentCount_ = componentCount;}

	bool IsFloating() const {return isFloating_;}

	void IsFloating(bool isFloating) {isFloating_ = isFloating;}

	bool IsIndices() const {return isIndices_;}

	void IsIndices(bool isIndices) {isIndices_ = isIndices;}
};

template <class DataType = Position>
class VertexData : public GenericVertexData
{
	Array <DataType> data_;
public:
	virtual void* GetData() {return (void*)&data_;}

	virtual int GetMemoryCapacity() {return data_.getMemoryCapacity();}
};

class Mesh
{
private:
	Map <GenericVertexData*> vertexDatas_;

	GLuint indexCount_;

	GLuint vertexCount_;

public:
	Mesh();

	void Initialize(int);

	template <class DataType>
	Array<DataType>* AddVertexData(const char*, int, bool, bool);

	template <class DataType>
	Array<DataType>* GetVertexDataArray(const char*);

	template <class DataType>
	VertexData<DataType>* GetVertexData(const char*);

	GenericVertexData* GetVertexData(const char*);

	int GetVertexDataMemoryCapacity(const char*);

	void* GetVertexDataStart(const char*);

	template <class DataType>
	DataType GetElement(const char*, int);

	Map <GenericVertexData*> &GetVertexDatas() {return vertexDatas_;}

	virtual ~Mesh();

	static Mesh* GenerateQuad();
};

class MeshAtlas
{
private:
	Map <Mesh> meshes_;

public:
	void AddMesh(const char*) {}

	void Initialize(const char*, int);

	Mesh& operator[](const char* name) {return *meshes_.Get(name);}
};
