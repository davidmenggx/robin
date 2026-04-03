#include "color/accumulation.h"
#include "color/color.h"
#include "constants.h"
#include "render/tonemap.h"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <vector>

using namespace ff;

uint8_t ff::RenderToRgb(float channel, float normalized_log_frequency) {
	// exposure scaling
	float output{ channel * normalized_log_frequency };

	// gamma correction
	output = std::pow(output, 1.0 / 2.2);

	// quantization
	return static_cast<uint8_t>(std::clamp(output * 255.0, 0.0, 255.0));
}

std::vector<Color> ff::GenerateTonemap(Accumulation& buffer) {
	constexpr int image_size{ constants::kWidth * constants::kHeight };

	std::vector<Color> output(image_size, { 0, 0, 0 });

	int max_frequency{ buffer.GetMaxFrequency() };
	// empty image
	if (max_frequency == 0) {
		return output;
	}

	for (int y{ 0 }; y < constants::kHeight; ++y) {
		for (int x{ 0 }; x < constants::kWidth; ++x) {
			PixelAccumulation& pixel{ buffer.Get(x, y) };
			if (pixel.frequency_ == 0) {
				continue;
			}

			int output_idx{ y * constants::kWidth + x };

			// brightness coefficient
			float normalized_log_frequency{ static_cast<float>(std::log(pixel.frequency_ + 1.0f)
						/ std::log(max_frequency + 1.0f)) };

			output[output_idx] = {
				RenderToRgb(pixel.red_, normalized_log_frequency),
				RenderToRgb(pixel.green_, normalized_log_frequency),
				RenderToRgb(pixel.blue_, normalized_log_frequency)
			};
		}
	}
	return output;
}
