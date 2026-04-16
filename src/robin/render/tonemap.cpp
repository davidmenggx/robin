#include "robin/color/accumulation.h"
#include "robin/color/color.h"
#include "robin/constants.h"

#include "robin/render/tonemap.h"

#include <algorithm>
#include <atomic>
#include <cmath>
#include <cstdint>
#include <vector>

static uint8_t applyImageFilters(float channel, float normalized_log_frequency) {
    // channel is in 0-255 range, normalize to 0-1
    float output{ channel / 255.0f };

    // exposure scaling
    output *= normalized_log_frequency;

    // gamma correction — input must be in 0-1 for this to be correct
    output = std::pow(output, 1.0f / 2.2f);

    // scale back to 0-255 and quantize
    return static_cast<uint8_t>(std::clamp(output * 255.0f, 0.0f, 255.0f));
}

std::vector<Color> generateTonemap(Accumulation& buffer, int width, int height) {
    const int image_size{ width * height };
    std::vector<Color> output(image_size, { 0, 0, 0, 255 });

    const uint64_t max_freq{ buffer.getMaxColorFrequency() };
    if (max_freq == 0) { 
        return output; 
    }

    const float log_max{ std::log(static_cast<float>(max_freq) + 1.0f) };
    const float inv_scale{ 1.0f / static_cast<float>(constants::kFixedScale) };

    for (int y{ 0 }; y < height; ++y) {
        for (int x{ 0 }; x < width; ++x) {
            const int idx{ y * width + x };

            const uint64_t freq{ buffer.frequency_[idx].load(std::memory_order_relaxed) };
            if (freq == 0) { 
                continue; 
            }

            const uint64_t raw_r{ buffer.red_[idx].load(std::memory_order_relaxed) };
            const uint64_t raw_g{ buffer.green_[idx].load(std::memory_order_relaxed) };
            const uint64_t raw_b{ buffer.blue_[idx].load(std::memory_order_relaxed) };

            const float inv_freq{ 1.0f / static_cast<float>(freq) };
            const float normalized_log_freq{ std::log(static_cast<float>(freq) + 1.0f) / log_max };

            // recover 0–255 float from fixed-point accumulator
            const float base_r = static_cast<float>(raw_r) * inv_scale * inv_freq;
            const float base_g = static_cast<float>(raw_g) * inv_scale * inv_freq;
            const float base_b = static_cast<float>(raw_b) * inv_scale * inv_freq;

            output[idx] = {
                applyImageFilters(base_r, normalized_log_freq),
                applyImageFilters(base_g, normalized_log_freq),
                applyImageFilters(base_b, normalized_log_freq),
                255
            };
        }
    }
    return output;
}
