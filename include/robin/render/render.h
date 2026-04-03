#pragma once

#include "color/accumulation.h"
#include "color/color.h"

#include <cstdint>
#include <vector>

namespace ff {
	uint8_t RenderToRgb(float channel, float normalized_log_density);

	std::vector<Color> GenerateTonemap(Accumulation& buffer);
}
