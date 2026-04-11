#include "robin/color/accumulation.h"
#include "robin/color/color.h"
#include "robin/color/pixel_accumulation.h"
#include "robin/config.h"

#include <algorithm>
#include <utility>
#include <vector>

Accumulation::Accumulation(Config& config)
	: config_{ config }
	, gradient_lookup_(config.gradient_)
	, histogram_(config.gui_width_ * config.gui_height_)
{}

[[nodiscard]] PixelAccumulation& Accumulation::getPixelFrequency(int x, int y) {
	return histogram_[y * config_.gui_width_ + x];
}

[[nodiscard]] int Accumulation::getMaxColorFrequency() const {
	int max_frequency{};
	for (const auto& pixel : histogram_) {
		max_frequency = std::max(max_frequency, pixel.frequency_);
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
void Accumulation::incrementFrequency(float x, float y, float color) {
	auto [x_proj, y_proj] = projectToScreen(x, y);
	if (x_proj >= 0 && x_proj < config_.gui_width_
		&& y_proj >= 0 && y_proj < config_.gui_height_) {

		PixelAccumulation& pixel{ getPixelFrequency(x_proj, y_proj) };

		Color sample{ gradient_lookup_.sampleGradientColor(color) };

		pixel.red_ += sample.red_ / 255.0f;
		pixel.green_ += sample.green_ / 255.0f;
		pixel.blue_ += sample.blue_ / 255.0f;
		++pixel.frequency_;
	}
}

// master function used by the engine to merge the buffers of each of the workers
void Accumulation::merge(const Accumulation& local_buffer) {
	for (std::size_t i{ 0 }; i < histogram_.size(); ++i) {
		histogram_[i].red_ += local_buffer.histogram_[i].red_;
		histogram_[i].green_ += local_buffer.histogram_[i].green_;
		histogram_[i].blue_ += local_buffer.histogram_[i].blue_;
		histogram_[i].frequency_ += local_buffer.histogram_[i].frequency_;
	}
}

// local function used by the worker threads to clear and re-sync their state after each merge
void Accumulation::clear() {
	for (auto& pixel : histogram_) {
		pixel.red_ = 0.0f;
		pixel.green_ = 0.0f;
		pixel.blue_ = 0.0f;
		pixel.frequency_ = 0;
	}
}
