#pragma once

#include "robin/generation/transformation.h"

#include <vector>

struct AliasTable {
    std::vector<float> probabilities;
    std::vector<std::size_t> aliases;
};

AliasTable generateDistribution(const std::vector<Transformation>& transforms);

// Returns the first index in the CDF whose value is at least @cutoff
std::size_t findIndex(const AliasTable& table, float uniformZeroToOne, std::size_t uniformIndex);
