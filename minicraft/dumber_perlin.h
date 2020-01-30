#pragma once
#include "engine/noise/perlin.h"

class YDumberPerlin : public YPerlin
{
public:

	float maxHeight = 50.0f;
	int minHeight = 0;

	
	virtual ~YDumberPerlin() = default;
	float sample(float xBase, float yBase, float zBase) override
	{
		//if (zBase >= maxHeight) return 0;


		Freq = 0.015625f;
		float val = YPerlin::sample(xBase, yBase, zBase) * 64;

		Freq = 0.03125f;
		val += YPerlin::sample(xBase, yBase, zBase) * 32;

		Freq = 0.0625f;
		val += YPerlin::sample(xBase, yBase, zBase) * 16;
		Freq = 0.125f;
		val += YPerlin::sample(xBase, yBase, zBase) * 8;

		return val / 120;
	}
};
