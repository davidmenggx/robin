#include "robin/color/accumulation.h"
#include "robin/color/color.h"
#include "robin/color/gradient_point.h"
#include "robin/color/pixel_accumulation.h"
#include "robin/config.h"
#include "robin/constants.h"

#include <algorithm>
#include <utility>
#include <vector>

using namespace ff;

Accumulation::Accumulation(const Config& config, 
	std::vector<GradientPoint>& gradient_points)
	: config_{ config }
	, gradient_lookup_(gradient_points)
	, histogram_(config.gui_width_ * config.gui_height_)
{
}

[[nodiscard]] PixelAccumulation& Accumulation::get(int x, int y) {
	return histogram_[y * config_.gui_width_ + x];
}

[[nodiscard]] int Accumulation::getMaxFrequency() const {
	int max_frequency{};
	for (const auto& pixel : histogram_) {
		max_frequency = std::max(max_frequency, pixel.frequency_);
	}
	return max_frequency;
}

std::pair<int, int> Accumulation::toPixels(float x, float y) {
	return {
	  static_cast<int>(x * constants::kPixelsPerUnit + (config_.gui_width_ / 2.0f)),
	  static_cast<int>(y * constants::kPixelsPerUnit + (config_.gui_height_ / 2.0f))
	};
}

void Accumulation::accumulate(float x, float y, float color) {
	auto [x_proj, y_proj] = toPixels(x, y);
	if (x_proj >= 0 && x_proj < config_.gui_width_
		&& y_proj >= 0 && y_proj < config_.gui_height_) {

		PixelAccumulation& pixel = get(x_proj, y_proj);

		Color sample = gradient_lookup_.sample(color);

		float sample_red{ sample.red_ / 255.0f };
		float sample_green{ sample.green_ / 255.0f };
		float sample_blue{ sample.blue_ / 255.0f };

		pixel.red_ = static_cast<float>(pixel.red_ * pixel.frequency_ + sample_red) /
			(pixel.frequency_ + 1);
		pixel.green_ = static_cast<float>(pixel.green_ * pixel.frequency_ + sample_green) /
			(pixel.frequency_ + 1);
		pixel.blue_ = static_cast<float>(pixel.blue_ * pixel.frequency_ + sample_blue) /
			(pixel.frequency_ + 1);
		++pixel.frequency_;
	}
}
