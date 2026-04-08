#include "robin/color/accumulation.h"
#include "robin/config.h"
#include "robin/engine/cdf.h"
#include "robin/engine/engine.h"
#include "robin/generation/flame.h"
#include "robin/generation/point2d.h"
#include "robin/generation/transformation.h"
#include "robin/generation/variation.h"
#include "robin/render/frame_events.h"
#include "robin/render/renderer.h"
#include "robin/render/tonemap.h"
#include "robin/utils/save_image.h"

#include <chrono>
#include <cmath>
#include <iostream>
#include <random>
#include <ratio>
#include <vector>

static const Transformation& chooseRandomTransformation(
	const std::vector<float>& cdf,
	std::mt19937& generator,
	std::uniform_real_distribution<float>& distribution,
	const Flame& flame) {

	float random{ distribution(generator) };
	std::size_t transformation_idx{ findIndex(cdf, random) };

	return flame.transformations_[transformation_idx];
}

// Each Transformation object is composed of one or more variations, apply them and return the updated result
static Point2D applyVariations(const std::vector<Variation>& variations, float x, float y) {
	float radius{ std::sqrt(x * x + y * y) };
	float theta{ std::atan2(y, x) };
	float phi{ std::atan2(x, y) };

	float x_accumulated{};
	float y_accumulated{};

	for (const Variation& variation : variations) {
		Point2D updated_point{ applyVariation(variation.type_, x, y, radius, theta, phi) };
		x_accumulated += variation.weight_ * updated_point.x_;
		y_accumulated += variation.weight_ * updated_point.y_;
	}

	return { x_accumulated, y_accumulated };
}

static void runEngineIteration(const Flame& flame,
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
			chooseRandomTransformation(cdf, generator, distribution, flame) };

		// apply it
		// matrix multiplication:
		float x_affine = transformation.factors_.a_ * x +
			transformation.factors_.b_ * y + transformation.factors_.c_;
		float y_affine = transformation.factors_.d_ * x +
			transformation.factors_.e_ * y + transformation.factors_.f_;

		Point2D updated_point{ applyVariations(transformation.variations_, x_affine, y_affine) };
		x = updated_point.x_;
		y = updated_point.y_;

		color = (color + transformation.color_) * 0.5f;
	}

	for (int i{ 0 }; i < iterations; ++i) {
		const Transformation& transformation{
			chooseRandomTransformation(cdf, generator, distribution, flame) };

		float x_affine = transformation.factors_.a_ * x +
			transformation.factors_.b_ * y + transformation.factors_.c_;
		float y_affine = transformation.factors_.d_ * x +
			transformation.factors_.e_ * y + transformation.factors_.f_;

		Point2D updated_point{ applyVariations(transformation.variations_, x_affine, y_affine) };
		x = updated_point.x_;
		y = updated_point.y_;

		color = (color + transformation.color_) * 0.5f;

		buffer.incrementFrequency(x, y, color);
	}
}

void runEngine(Config& config) {
	Flame flame{ config.transformation_ };

	std::vector<float> cdf{ generateDistribution(config.transformation_) };

	Accumulation buffer{ config };

	std::random_device device{};
	std::mt19937 generator(device());
	std::uniform_real_distribution<float> distribution{ 0, 1 };

	Renderer renderer{ "robin", config };

	int frame{};
	int total_points{};
	while (true) {
		FrameEvents events{ renderer.fetchUserInput() };

		if (events.quit_) {
			break;
		}
		if (events.save_) {
			if (!utils::saveImage(generateTonemap(buffer, config.gui_width_, 
				config.gui_height_), config)) {
				std::cerr << "Failed to save image output";
			}
		}

		auto t1 = std::chrono::steady_clock::now();
		runEngineIteration(flame, cdf, buffer, generator, distribution, config.iterations_);
		total_points += config.iterations_;
		auto t2 = std::chrono::steady_clock::now();

		float ms{ std::chrono::duration<float, std::milli>(t2 - t1).count() };
		float points_per_second{ config.iterations_ / (ms / 1000.0f) };

		renderer.updateTelemetry(total_points, points_per_second);

		if (frame % 10 == 0) {
			renderer.updateGUI(buffer);
		}
		++frame;
	}
}
