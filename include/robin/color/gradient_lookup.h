#pragma once

#include "robin/color/color.h"
#include "robin/color/gradient_point.h"
#include "robin/constants.h"

#include <array>
#include <vector>

namespace ff {
	class GradientLookup {
	public:
		GradientLookup() = delete;

		GradientLookup(std::vector<GradientPoint>& gradient_points);

		Color sample(float color);

	private:
		std::array<Color, constants::kGradientLookupSize> lookup_table_{};
	};
}
