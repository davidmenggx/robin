#pragma once

#include "robin/generation/transformation.h"

#include <vector>

namespace ff {
	std::vector<float> generateCDF(const std::vector<Transformation>& transforms);
	std::size_t drawCDF(const std::vector<float>& cdf, float random);
}
