#pragma once

#include "robin/color/color.h"

#include <vector>

namespace ff {
	struct GradientPoint {
		float position_{}; // between 0 and 1 inclusive
		Color color_{};
	};

	// TODO: move to constants
	// TEMPORARY GRADIENT POINTS, MOVE SOMEWHERE ELSE!!!!:
	inline std::vector<GradientPoint> kGradientPoints = {
		{0.0f,   0,  15,  30},
		{0.35f, 10,  80, 160},
		{0.65f, 20, 220, 200},
		{1.0f, 220, 255, 255},
	};
}
