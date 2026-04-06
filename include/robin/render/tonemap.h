#pragma once

#include "robin/color/accumulation.h"
#include "robin/color/color.h"

#include <cstdint>
#include <vector>

namespace ff {
	uint8_t renderToRgb(float channel, float normalized_log_density);

	std::vector<Color> generateTonemap(Accumulation& buffer, int width, int height);
}
