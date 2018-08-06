#include "DataBuffer.hpp"
#include "Utility/Utility.hpp"

#include "Game/CultureModelBuilder.hpp"
#include "Game/SettlementRenderer.hpp"
#include "Game/World.hpp"
#include "Game/Settlement.hpp"
#include "Game/Types.hpp"

void CultureModelBuilder::Generate(World& world)
{
	auto & settlements = world.GetSettlements();

	auto capacity = settlements.GetSize() * 30;
	auto & buildingDatas = SettlementRenderer::GetBuildingDatas();
	buildingDatas.Initialize(capacity);

	auto & settlementDatas = SettlementRenderer::GetSettlementDatas();
	settlementDatas.Initialize(settlements.GetSize());

	Index buildingIndex = 0;
	for(auto settlement = settlements.GetStart(); settlement != settlements.GetEnd(); ++settlement)
	{
		auto buildingCount = utility::GetRandom(5, 30);

		auto settlementData = settlementDatas.Allocate();
		settlementData->BuildingIndex_ = buildingIndex;
		settlementData->BuildingCount_ = buildingCount;

		settlement->SetRenderData(settlementData);

		for(int i = 0; i < buildingCount; ++i)
		{
			BuildingRenderData* buildingData = buildingDatas.Allocate();

			Float x = utility::GetRandom(-1.5f, 1.5f);
			Float y = utility::GetRandom(-1.5f, 1.5f);
			buildingData->Position_ = Float3(settlement->GetPosition(), 0.0f) + Float3(x, y, 0.0f);
			buildingData->Position_.z = world.GetHeight(buildingData->Position_.x, buildingData->Position_.y);

			buildingData->Rotation_ = utility::GetRandom(0.0f, 6.2831f);

			buildingData->MeshIndex_ = utility::GetRandom(0, 2);

			buildingIndex++;
		}
	}

	auto & modelBuffers = SettlementRenderer::GetBuffers();

	auto buffer = modelBuffers.Add(SettlementModelBuffers::BUILDING_DATAS);
	*buffer = new DataBuffer(buildingDatas.GetMemorySize(), buildingDatas.GetStart());
}
