#include "robin/color/accumulation.h"
#include "robin/color/color.h"
#include "robin/config.h"
#include "robin/constants.h"

#include <algorithm>
#include <cstdint>
#include <utility>

Accumulation::Accumulation(Config& config)
	: config_{ config }
	, gradient_lookup_(config.gradient_)
	, frequency_(config.gui_width_* config.gui_height_)
	, red_(config.gui_width_* config.gui_height_)
	, green_(config.gui_width_* config.gui_height_)
	, blue_(config.gui_width_* config.gui_height_)
{
}

// TODO: make this more performant
[[nodiscard]] uint64_t Accumulation::getMaxColorFrequency() const {
	uint64_t max_frequency{};
	for (const auto& freq : frequency_) {
		max_frequency = std::max(max_frequency, freq.load());
	}
	return max_frequency;
}

std::pair<int, int> Accumulation::projectToScreen(float x, float y) {
	return {
	  static_cast<int>((x - config_.camera_center_x_) * config_.camera_scale_
						 + config_.gui_width_ / 2.0f),
		static_cast<int>((y - config_.camera_center_y_) * config_.camera_scale_
						 + config_.gui_height_ / 2.0f)
	};
}

// local function used by the worker thread to update local histogram
void Accumulation::markPixelHit(float x, float y, float color) {
	auto [x_proj, y_proj] = projectToScreen(x, y);
	if (x_proj >= 0 && x_proj < config_.gui_width_
		&& y_proj >= 0 && y_proj < config_.gui_height_) {

		const int idx{ y_proj * config_.gui_width_ + x_proj };
		Color sample{ gradient_lookup_.sampleGradientColor(color) };

		red_[idx].fetch_add(static_cast<uint64_t>(sample.red_ * constants::kFixedScale), std::memory_order_relaxed);
		green_[idx].fetch_add(static_cast<uint64_t>(sample.green_ * constants::kFixedScale), std::memory_order_relaxed);
		blue_[idx].fetch_add(static_cast<uint64_t>(sample.blue_ * constants::kFixedScale), std::memory_order_relaxed);
		frequency_[idx].fetch_add(1, std::memory_order_relaxed);
	}
}
