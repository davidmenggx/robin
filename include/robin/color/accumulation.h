#pragma once

#include "robin/color/gradient_lookup.h"
#include "robin/color/gradient_point.h"
#include "robin/color/pixel_accumulation.h"
#include "robin/config.h"

#include <utility>
#include <vector>

namespace ff {
	class Accumulation {
	public:
		explicit Accumulation(const Config& config);

		[[nodiscard]] PixelAccumulation& get(int x, int y);

		[[nodiscard]] int getMaxFrequency() const;

		void accumulate(float x, float y, float color);

	private:
		const Config& config_;

		std::vector<PixelAccumulation> histogram_{};

		GradientLookup gradient_lookup_;

		std::pair<int, int> toPixels(float x, float y);
	};
}

