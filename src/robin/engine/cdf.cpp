#include "robin/engine/cdf.h"
#include "robin/generation/transformation.h"

#include <iterator>
#include <vector>

using namespace ff;

std::vector<float> ff::generateCDF(const std::vector<Transformation>& transforms) {
	// compute total weight to normalize the proportion that each transform is
	// chosen later
	float total{};
	for (const Transformation& transform : transforms) {
		total += transform.weight_;
	}

	std::vector<float> cdf{};
	cdf.reserve(std::size(transforms));
	float cumulative{};
	for (const Transformation& transform : transforms) {
		// remember to normalize the weights
		cumulative += transform.weight_ / total;
		cdf.push_back(cumulative);
	}

	// prevent floating point error accumulation bugs
	// the weighted sum of the proportion of all transformations should be 1
	cdf.back() = 1.0;
	return cdf;
}

std::size_t ff::drawCDF(const std::vector<float>& cdf, float random) {
	// Rn this is a naive linear search, improve to better search algorithm
	for (std::size_t i{ 0 }; i < std::size(cdf); ++i) {
		if (random <= cdf[i]) {
			return i;
		}
	}
	return std::size(cdf) - 1;
}
