#include "robin/color/accumulation.h"
#include "robin/color/color.h"
#include "robin/color/pixel_accumulation.h"
#include "robin/render/tonemap.h"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <vector>

static uint8_t applyImageFilters(float channel, float normalized_log_frequency) {
	// exposure scaling
	float output{ channel * normalized_log_frequency };

	// gamma correction
	output = std::pow(output, 1.0 / 2.2);

	// quantization
	return static_cast<uint8_t>(std::clamp(output * 255.0, 0.0, 255.0));
}

std::vector<Color> generateTonemap(Accumulation& buffer, int width, int height) {
	const int image_size{ width * height };

	std::vector<Color> output(image_size, { 0, 0, 0, 255 });

	int max_frequency{ buffer.getMaxColorFrequency() };

	// empty image
	if (max_frequency == 0) {
		return output;
	}

	for (int y{ 0 }; y < height; ++y) {
		for (int x{ 0 }; x < width; ++x) {
			PixelAccumulation& pixel{ buffer.getPixelFrequency(x, y) };
			if (pixel.frequency_ == 0) {
				continue;
			}

			int output_idx{ y * width + x };

			// brightness coefficient
			float normalized_log_frequency{ static_cast<float>(std::log(pixel.frequency_ + 1.0f)
						/ std::log(max_frequency + 1.0f)) };

			float base_r{ pixel.red_ / pixel.frequency_ };
			float base_g{ pixel.green_ / pixel.frequency_ };
			float base_b{ pixel.blue_ / pixel.frequency_ };

			output[output_idx] = {
				applyImageFilters(base_r, normalized_log_frequency),
				applyImageFilters(base_g, normalized_log_frequency),
				applyImageFilters(base_b, normalized_log_frequency),
				255
			};
		}
	}
	return output;
}
