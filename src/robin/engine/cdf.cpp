#include "engine/cdf.h"
#include "generation/transformation.h"

#include <vector>

using namespace ff;

std::vector<float> ff::GenerateCDF(
  const std::vector<Transformation>& transforms) {
  // compute total weight to normalize the proportion that each transform is
  // chosen later
  double total{};
  for (const Transformation& t : transforms) {
    total += t.weight_;
  }

  std::vector<float> cdf{};
  cdf.reserve(std::size(transforms));
  float cumulative{};
  for (const Transformation& t : transforms) {
    // remember to normalize the weights
    cumulative += t.weight_ / total;
    cdf.push_back(cumulative);
  }

  // prevent floating point error accumulation bugs
  // the weighted sum of the proportion of all transformations should be 1
  cdf.back() = 1.0;
  return cdf;
}

std::size_t ff::DrawCDF(const std::vector<float> cdf, float random) {
  // Rn this is a naive linear search, improve to better search algorithm

  for (std::size_t i{0}; i < std::size(cdf); ++i) {
    if (random > cdf[i]) {
      return i;
    }
  }
  return std::size(cdf) - 1;
}
