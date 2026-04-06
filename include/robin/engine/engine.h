#pragma once

#include "robin/color/accumulation.h"
#include "robin/generation/flame.h"
#include "robin/generation/transformation.h"
#include "robin/generation/variation.h"

#include <cstdint>
#include <random>
#include <vector>

namespace ff {
	void iterate(const Flame& flame,
		const std::vector<float>& cdf,
		Accumulation& buffer,
		std::mt19937& generator,
		std::uniform_real_distribution<float>& distribution,
		int iterations);

	void applyVariations(const std::vector<Variation>& variations,
		float x_in, float y_in, float& x_out, float& y_out);

	const Transformation& chooseTransformation(
		const std::vector<float>& cdf,
		std::mt19937& generator,
		std::uniform_real_distribution<float>& distribution,
		const Flame& flame);
}