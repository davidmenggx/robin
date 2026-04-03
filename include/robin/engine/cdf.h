#pragma once

#include "generation/transformation.h"

#include <vector>

namespace ff {
	std::vector<float> GenerateCDF(const std::vector<Transformation>& transforms);
	std::size_t DrawCDF(const std::vector<float> cdf, float random);
}
