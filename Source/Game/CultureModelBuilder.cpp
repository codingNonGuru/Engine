#include "DataBuffer.hpp"
#include "Utility/Utility.hpp"
#include "Utility/Perlin.hpp"

#include "Game/CultureModelBuilder.hpp"
#include "Game/SettlementRenderer.hpp"
#include "Game/World.hpp"
#include "Game/Settlement.hpp"
#include "Game/Types.hpp"

void CultureModelBuilder::Generate(World& world)
{
	Grid <Float> buildingMap (64, 64);

	auto noiseMapSize = Size(2048, 2048);
	Grid <Float> noiseMap(noiseMapSize.x, noiseMapSize.y);
	Perlin::Generate(noiseMapSize, FocusIndex(0.6f), ContrastThreshold(0.5f), ContrastStrength(4.0f));
	Perlin::Download(&noiseMap);

	auto & settlements = world.GetSettlements();

	auto capacity = settlements.GetSize() * 1000;
	auto & buildingDatas = SettlementRenderer::GetBuildingDatas();
	buildingDatas.Initialize(capacity);

	auto & settlementDatas = SettlementRenderer::GetSettlementDatas();
	settlementDatas.Initialize(settlements.GetSize());

	Index buildingIndex = 0;
	for(auto settlement = settlements.GetStart(); settlement != settlements.GetEnd(); ++settlement)
	{
		Size offset = Size(utility::GetRandom(0, noiseMapSize.x), utility::GetRandom(0, noiseMapSize.y));

		auto startIndex = buildingIndex;
		auto buildingCount = 0;

		const int halfSize = buildingMap.GetWidth() / 2;
		for(int x = 0; x < buildingMap.GetWidth(); ++x)
		{
			for(int y = 0; y < buildingMap.GetHeight(); ++y)
			{
				float populationFactor = (Float)settlement->GetPopulation() / 120.0f;

				float distanceFactor = float(x - halfSize) * float(x - halfSize) + float(y - halfSize) * float(y - halfSize);
				float heightFactor = distanceFactor;
				distanceFactor = exp(-distanceFactor / (2.0f * populationFactor * populationFactor));

				float chance = *noiseMap.Get(offset.x + x, offset.y + y);
				chance = (chance * 0.5f + distanceFactor * 0.5f) * distanceFactor;

				if(chance < 0.5f)
					continue;

				BuildingRenderData* buildingData = buildingDatas.Allocate();

				Float finalX = (Float(x) + (y % 2 == 0 ? 0.3f : 0.0f) - Float(halfSize)) * 0.2f;
				Float finalY = (Float(y) + (x % 2 == 0 ? 0.3f : 0.0f) - Float(halfSize)) * 0.2f;
				float scatterFactor = 2.0f - pow(distanceFactor, 1.5f);
				finalX *= scatterFactor;
				finalY *= scatterFactor;

				float angle = utility::GetRandom(0.0f, 6.2831f);
				float radius = utility::GetRandom(0.0f, 0.04f);
				finalX += cos(angle) * radius * scatterFactor;
				finalY += sin(angle) * radius * scatterFactor;

				buildingData->Position_ = Float3(settlement->GetPosition(), 0.0f) + Float3(finalX, finalY, 0.0f);
				buildingData->Position_.z = world.GetHeight(buildingData->Position_.x, buildingData->Position_.y);

				buildingData->Rotation_ = utility::GetRandom(0.0f, 6.2831f);

				heightFactor = exp(-heightFactor / (0.5f * populationFactor * populationFactor));
				buildingData->MeshIndex_ = heightFactor * 3.5f;

				buildingIndex++;
				buildingCount++;
			}
		}

		auto settlementData = settlementDatas.Allocate();
		settlementData->BuildingIndex_ = startIndex;
		settlementData->BuildingCount_ = buildingCount;

		settlement->SetRenderData(settlementData);
	}

	auto & modelBuffers = SettlementRenderer::GetBuffers();

	auto buffer = modelBuffers.Add(SettlementModelBuffers::BUILDING_DATAS);
	*buffer = new DataBuffer(buildingDatas.GetMemorySize(), buildingDatas.GetStart());
}
