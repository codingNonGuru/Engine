#include "Utility/Perlin.hpp"

#include "Shader.hpp"
#include "ShaderManager.hpp"
#include "DataBuffer.hpp"
#include "Utility/Utility.hpp"

Shader* Perlin::shader_ = nullptr;

Map <DataBuffer> Perlin::buffers_ = Map <DataBuffer> (3);

DataBuffer* Perlin::targetBuffer_ = nullptr;

bool Perlin::isInitialized_ = false;

enum class Stages
{
	SET_LATTICE_CORE,
	SET_LATTICE_EDGES,
	FILL_LATTICE,
	ADD_OCTAVES,
	CLEAR
};

void Perlin::Initialize()
{
	if(isInitialized_)
	{
		return;
	}

	isInitialized_ = true;

	clock_t start = clock();

	Size capacity(4096, 4096);
	Length pixelCount = capacity.x * capacity.y;

	container::Array<float, int> stream(pow(2, 24));

	for(int index = 0; index < stream.GetCapacity(); ++index)
	{
		auto pixel = stream.Allocate();
		*pixel = utility::GetRandom(0.0f, 1.0f);
	}

	*buffers_.Add("octaves") = DataBuffer(pixelCount * sizeof(float));
	*buffers_.Add("result") = DataBuffer(pixelCount * sizeof(float));
	*buffers_.Add("stream") = DataBuffer(stream.GetMemoryCapacity(), stream.GetStart());

	shader_ = ShaderManager::GetShaderMap().Get("Perlin");

	std::cout<<clock() - start<<"\n";
}

void Perlin::Destroy()
{
	for(auto buffer = buffers_.GetStart(); buffer != buffers_.GetEnd(); ++buffer)
	{
		buffer->Delete();
	}
}

void Perlin::SetTargetBuffer(DataBuffer* targetBuffer)
{
	targetBuffer_ = targetBuffer;
}

DataBuffer* Perlin::Generate(Size size, FocusIndex focusIndex, ContrastThreshold contrastThreshold, ContrastStrength contrastStrength, Range range)
{
	Initialize();

	clock_t start = clock();

	int blockSize = 16;
	Size computeSize(size.x / blockSize, size.y / blockSize);

	shader_->Bind();

	buffers_.Get("octaves")->Bind(0);
	if(targetBuffer_ != nullptr)
	{
		targetBuffer_->Bind(1);
		targetBuffer_ = nullptr;
	}
	else
	{
		buffers_.Get("result")->Bind(1);
	}
	buffers_.Get("stream")->Bind(2);

	shader_->SetConstant(size, "size");
	auto stage = (int)Stages::CLEAR;
	shader_->SetConstant(stage, "stage");
	shader_->DispatchCompute(computeSize);

	auto side = size.x;
	int horizontalExponent = 0;
	while(side % 2 == 0)
	{
		side /= 2;
		horizontalExponent++;
	}

	side = size.y;
	int verticalExponent = 0;
	while(side % 2 == 0)
	{
		side /= 2;
		verticalExponent++;
	}

	int exponent = verticalExponent > horizontalExponent ? horizontalExponent : verticalExponent;

	//Generate layers and add them to result
	unsigned int order = pow(2, exponent);
	unsigned int octaveCount = exponent;
	shader_->SetConstant(octaveCount, "octaveCount");

	for(int tier = octaveCount; tier >= 0; --tier, order /= 2)
	{
		shader_->SetConstant(order, "order");

		shader_->SetConstant(tier, "octaveIndex");

		unsigned int timeSeed = utility::GetRandom(0, pow(2, 24));
		shader_->SetConstant(timeSeed, "timeSeed");

		shader_->SetConstant(focusIndex, "strongestOctave");

		for(Index stage = 0; stage < 3; ++stage)
		{
			shader_->SetConstant(stage, "stage");
			shader_->DispatchCompute(computeSize);
		}
	}

	//Finish result
	stage = (int)Stages::ADD_OCTAVES;
	shader_->SetConstant(stage, "stage");
	shader_->SetConstant(octaveCount, "octaveCount");
	shader_->SetConstant(range, "range");
	shader_->SetConstant(contrastThreshold, "contrast");
	shader_->SetConstant(contrastStrength, "contrastStrength");

	shader_->DispatchCompute(computeSize);

	shader_->Unbind();

	std::cout<<"Finished generating Perlin texture on the GPU of size "<<size.x<<"x"<<size.y<<" in time "<<clock() - start<<" nanosecs.\n";

	return GetResultBuffer();
}

void Perlin::Download(container::Matrix* container)
{
	buffers_.Get("result")->Download(container);
}

DataBuffer* Perlin::GetResultBuffer()
{
	return buffers_.Get("result");
}
