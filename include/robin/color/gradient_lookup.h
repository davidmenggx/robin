#pragma once

#include "color/color.h"
#include "color/gradient_point.h"
#include "constants.h"

#include <array>
#include <vector>

namespace ff {
	class GradientLookup {
	public:
		GradientLookup() = delete;

		GradientLookup(std::vector<GradientPoint>& gradient_points);

		Color Sample(float color);

	private:
		std::array<Color, constants::kGradientLookupSize> lookup_table_{};
	};
}
