#pragma once

#include "robin/generation/transformation.h"

#include <vector>

std::vector<float> generateDistribution(const std::vector<Transformation>& transforms);

// Returns the first index in the CDF whose value is at least @cutoff
std::size_t findIndex(const std::vector<float>& cdf, float cutoff);
