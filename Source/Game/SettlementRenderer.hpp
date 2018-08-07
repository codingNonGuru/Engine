#pragma once

#include "Model.hpp"

struct BuildingRenderData
{
	Float3 Position_;

	Float Rotation_;

	Index MeshIndex_;
};

struct SettlementRenderData
{
	Index BuildingIndex_;

	Index BuildingCount_;
};

class SettlementRenderer : public Model
{
	static SettlementRenderer* instance_;

	Array <BuildingRenderData> buildingDatas_;

	Array <SettlementRenderData> settlementDatas_;

	Array <Index> buildingIndices_;

	Length defaultMeshSize_;

	static SettlementRenderer* GetInstance();

	SettlementRenderer();

	virtual void Render(Camera*, Light*);

	virtual void RenderShadows(Camera*, Light*);

	void ProcessData(Camera*);

	void AssembleMesh();

public:
	static void Update(Camera*, Light*);

	static void ProjectShadows(Camera*, Light*);

	static Array <SettlementRenderData> & GetSettlementDatas();

	static Array <BuildingRenderData> & GetBuildingDatas();

	static Map <DataBuffer*> & GetBuffers();

	static const int BUILDING_RENDER_CAPACITY;
};