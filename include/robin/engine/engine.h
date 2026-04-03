#pragma once

#include "color/accumulation.h"
#include "generation/flame.h"
#include "generation/transformation.h"
#include "generation/variation.h"

#include <cstdint>
#include <random>
#include <vector>

namespace ff {
  void Iterate(const Flame& flame, 
             const std::vector<float>& cdf,
             Accumulation& buffer, 
             std::mt19937& generator,
             std::uniform_real_distribution<float>& distribution,
             int iterations);

  void ApplyVariations(const std::vector<Variation>& variations, 
             float x_in, float y_in, float& x_out, float& y_out);

  const Transformation& ChooseTransformation(
             const std::vector<float>& cdf, 
             std::mt19937& generator,
             std::uniform_real_distribution<float>& distribution, 
             const Flame& flame);
}