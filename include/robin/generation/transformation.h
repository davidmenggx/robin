#pragma once

#include "robin/generation/affine.h"
#include "robin/generation/variation.h"

#include <vector>

struct Transformation {
	// each transformation is the weighted sum of multiple variations
	std::vector<Variation> variations_{};

	Affine factors_{};
	float weight_{};  // proportion of time that this transformation is picked
	float color_{}; // 0.0 to 1.0 representing the index of a gradient
};
