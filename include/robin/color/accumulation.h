#pragma once

#include "color/gradient_lookup.h"
#include "color/gradient_point.h"
#include "color/pixel_accumulation.h"

#include <utility>
#include <vector>

namespace ff {
	class Accumulation {
	public:
		explicit Accumulation(
			std::vector<GradientPoint>& gradient_points = kGradientPoints);

		[[nodiscard]] PixelAccumulation& Get(int x, int y);

		void Accumulate(float x, float y, float color);

		// THIS IS PUBLIC TEMPORARILY FOR DEBUG PURPOSES!!!
		std::vector<PixelAccumulation> histogram_{};
	private:
		GradientLookup gradient_lookup_;

		std::pair<int, int> ToPixels(float x, float y);
	};
}

