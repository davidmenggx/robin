#pragma once

#include "render/color.h"

#include <cstdint>

namespace constants {
	// gui details
	inline constexpr int kWidth{ 1000 };
	inline constexpr int kHeight{ 1000 };
	inline constexpr render::Color kBackground{ 0, 0, 0 };

	// TODO: replace this with some default command line arg
	inline constexpr int kIterations{ 100000 };

	inline constexpr float kPixelsPerUnit{ 500.0f };
	inline constexpr int kGradientLookupSize{ 1024 };
}
