#include "robin/color/accumulation.h"
#include "robin/config.h"
#include "robin/engine/cdf.h"
#include "robin/engine/engine.h"
#include "robin/generation/flame.h"
#include "robin/generation/transformation.h"
#include "robin/generation/variation.h"
#include "robin/render/frame_events.h"
#include "robin/render/renderer.h"
#include "robin/render/tonemap.h"
#include "robin/utils/save_image.h"

#include <chrono>
#include <cmath>
#include <random>
#include <ratio>
#include <vector>

using namespace ff;

const Transformation& ff::chooseTransformation(
	const std::vector<float>& cdf,
	std::mt19937& generator,
	std::uniform_real_distribution<float>& distribution,
	const Flame& flame) {

	float random{ distribution(generator) };
	std::size_t transformation_idx{ drawCDF(cdf, random) };

	return flame.transformations_[transformation_idx];
}

void ff::applyVariations(
	const std::vector<Variation>& variations,
	float x_in, float y_in, float& x_out, float& y_out) {
	float radius{ std::sqrt(x_in * x_in + y_in * y_in) };
	float theta{ std::atan2(y_in, x_in) };
	float phi{ std::atan2(x_in, y_in) };

	float x_accumulated{}, y_accumulated{};

	for (const Variation& variation : variations) {
		float x_var{}, y_var{};
		applyVariation(variation.type_, x_in, y_in, x_var, y_var, radius, theta, phi);
		x_accumulated += variation.weight_ * x_var;
		y_accumulated += variation.weight_ * y_var;
	}

	x_out = x_accumulated;
	y_out = y_accumulated;
}

void ff::iterate(const Flame& flame,
	const std::vector<float>& cdf,
	Accumulation& buffer,
	std::mt19937& generator,
	std::uniform_real_distribution<float>& distribution,
	int iterations) {
	// it doesn't rlly matter where we start at (see next lines)
	// for now our point starts with -1 <= x, y <= 1
	float x{ distribution(generator) * 2.0f - 1.0f };
	float y{ distribution(generator) * 2.0f - 1.0f };
	float color{ distribution(generator) };

	// our random initialization of the point means that the first few
	// iterations will quickly converge to some pattern, but this should
	// not be included in the final drawing (b/c it's ugly).
	// So I set the point in motion without accumulating color to settle
	// into some converged state
	for (int i{ 0 }; i < 20; ++i) {
		const Transformation& transformation{
			chooseTransformation(cdf, generator, distribution, flame) };

		// apply it
		// matrix multiplication:
		float x_affine = transformation.factors_.a_ * x +
			transformation.factors_.b_ * y + transformation.factors_.c_;
		float y_affine = transformation.factors_.d_ * x +
			transformation.factors_.e_ * y + transformation.factors_.f_;

		applyVariations(transformation.variations_, x_affine, y_affine, x, y);

		color = (color + transformation.color_) * 0.5f;
	}

	for (int i{ 0 }; i < iterations; ++i) {
		const Transformation& transformation{
			chooseTransformation(cdf, generator, distribution, flame) };

		float x_affine = transformation.factors_.a_ * x +
			transformation.factors_.b_ * y + transformation.factors_.c_;
		float y_affine = transformation.factors_.d_ * x +
			transformation.factors_.e_ * y + transformation.factors_.f_;

		applyVariations(transformation.variations_, x_affine, y_affine, x, y);

		color = (color + transformation.color_) * 0.5f;

		buffer.accumulate(x, y, color);
	}
}

void ff::runEngine(const Config& config) {
	Flame flame{ config.transformation_ };

	std::vector<float> cdf{ generateCDF(config.transformation_) };

	Accumulation buffer{ config };

	std::random_device device{};
	std::mt19937 generator(device());
	std::uniform_real_distribution<float> distribution{ 0, 1 };

	Renderer renderer{ "robin", config };

	int frame{};
	int total_points{};
	for (;;) {
		FrameEvents events{ renderer.pollEvents() };

		if (events.quit_) {
			break;
		}
		if (events.save_) {
			if (!utils::saveImage(generateTonemap(buffer, config.gui_width_, 
				config.gui_height_), config)) {
				// TODO: Add some logging here
			}
		}

		auto t1 = std::chrono::steady_clock::now();

		iterate(flame, cdf, buffer, generator, distribution, config.iterations_);
		total_points += config.iterations_;

		auto t2 = std::chrono::steady_clock::now();
		float ms{ static_cast<float>(std::chrono::duration<float, std::milli>(t2 - t1).count()) };
		float points_per_second{ static_cast<float>(config.iterations_ / (ms / 1000.0)) };

		renderer.updateTelemetry(total_points, points_per_second);
		renderer.update(buffer);

		if (frame % 10 == 0) {
			renderer.update(buffer);
		}
		++frame;
	}
}
