#include "color/accumulation.h"
#include "color/color.h"
#include "color/gradient_point.h"
#include "constants.h"

#include <utility>

using namespace ff;

Accumulation::Accumulation(std::vector<GradientPoint>& gradient_points)
	: histogram_(constants::kWidth* constants::kHeight)
	, gradient_lookup_(gradient_points) {
}

[[nodiscard]] PixelAccumulation& Accumulation::Get(int x, int y) {
	return histogram_[y * constants::kWidth + x];
}

std::pair<int, int> Accumulation::ToPixels(float x, float y) {
	return {
	  static_cast<int>(x * constants::kPixelsPerUnit),
	  static_cast<int>(y * constants::kPixelsPerUnit)
	};
}

void Accumulation::Accumulate(float x, float y, float color) {
	auto [x_proj, y_proj] = ToPixels(x, y);
	if (x_proj >= 0 && x_proj < constants::kWidth
		&& y_proj >= 0 && y_proj < constants::kHeight) {

		PixelAccumulation& pixel = Get(x_proj, y_proj);

		Color sample = gradient_lookup_.Sample(color);

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
