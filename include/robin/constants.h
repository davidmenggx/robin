#pragma once

#include "render/color.h"

namespace constants {
	// gui details
	inline constexpr int kWidth{ 1500 };
	inline constexpr int kHeight{ 1500 };
	inline constexpr render::Color kBackground{ 0, 0, 0 };

	// TODO: replace this with some default command line arg
	inline constexpr int kIterations{ 10'000'000 };
	inline constexpr int kIterationsPerUpdate{ 10'000 };

	// rendering settings:
	inline constexpr float kPixelsPerUnit{ 750.0f };
	inline constexpr int kGradientLookupSize{ 1024 };
}
