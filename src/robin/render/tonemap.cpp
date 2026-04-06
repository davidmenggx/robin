#include "color/accumulation.h"
#include "color/color.h"
#include "color/pixel_accumulation.h"
#include "constants.h"
#include "render/tonemap.h"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <vector>

using namespace ff;

uint8_t ff::renderToRgb(float channel, float normalized_log_frequency) {
	// exposure scaling
	float output{ channel * normalized_log_frequency };

	// gamma correction
	output = std::pow(output, 1.0 / 2.2);

	// quantization
	return static_cast<uint8_t>(std::clamp(output * 255.0, 0.0, 255.0));
}

std::vector<Color> ff::generateTonemap(Accumulation& buffer, int width, int height) {
	const int image_size{ width * height };

	std::vector<Color> output(image_size, { 0, 0, 0 });

	int max_frequency{ buffer.getMaxFrequency() };
	// empty image
	if (max_frequency == 0) {
		return output;
	}

	for (int y{ 0 }; y < height; ++y) {
		for (int x{ 0 }; x < width; ++x) {
			PixelAccumulation& pixel{ buffer.get(x, y) };
			if (pixel.frequency_ == 0) {
				continue;
			}

			int output_idx{ y * width + x };

			// brightness coefficient
			float normalized_log_frequency{ static_cast<float>(std::log(pixel.frequency_ + 1.0f)
						/ std::log(max_frequency + 1.0f)) };

			output[output_idx] = {
				renderToRgb(pixel.red_, normalized_log_frequency),
				renderToRgb(pixel.green_, normalized_log_frequency),
				renderToRgb(pixel.blue_, normalized_log_frequency)
			};
		}
	}
	return output;
}
