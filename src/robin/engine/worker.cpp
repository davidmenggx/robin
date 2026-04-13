#include "robin/color/accumulation.h"
#include "robin/config.h"
#include "robin/distribution/alias_distribution.h"
#include "robin/distribution/xoshiro.h"
#include "robin/engine/worker.h"
#include "robin/generation/flame.h"
#include "robin/generation/point2d.h"
#include "robin/generation/transformation.h"
#include "robin/generation/variation.h"

#include <cstdint>
#include <functional>
#include <limits>
#include <stop_token>
#include <thread>
#include <utility>
#include <vector>

static const Transformation& chooseRandomTransformation(
	AliasTable& alias,
	Xoshiro256& generator,
	float alias_scale,
	const Flame& flame) {

	// our findIndex function requires two random numbers: an index (size t) and a float
	// instead i just randomly generate one float and truncate it appropraitely to extract
	// both values
	// C++
	double uniform01{ static_cast<double>(generator()) / (static_cast<double>(std::numeric_limits<uint64_t>::max()) + 1.0) };
	float combined_random = static_cast<float>(uniform01 * alias_scale);

	std::size_t uniform_idx = static_cast<std::size_t>(combined_random);
	if (uniform_idx >= alias.probabilities.size()) {
		uniform_idx = alias.probabilities.size() - 1;
	}

	float uniform_float = combined_random - static_cast<float>(uniform_idx);

	std::size_t transformation_idx{ findIndex(alias, uniform_float, uniform_idx) };

	return flame.transformations_[transformation_idx];
}

// Each Transformation object is composed of one or more variations, apply them and return the updated result
static Point2D applyVariations(const std::vector<Variation>& variations, float x, float y) {

	float x_accumulated{};
	float y_accumulated{};

	for (const Variation& variation : variations) {
		Point2D updated_point{ applyVariation(variation.type_, x, y) };
		x_accumulated += variation.weight_ * updated_point.x_;
		y_accumulated += variation.weight_ * updated_point.y_;
	}

	return { x_accumulated, y_accumulated };
}

Worker::Worker(const Flame& flame, AliasTable& alias, Config& config,
	std::function<void(const Accumulation&, uint64_t)> flush_callback)
	: flame_{ flame }, alias_{ alias }, config_{ config }, buffer_{ config_ }
	, flush_callback_{ std::move(flush_callback) }, generator_{ 5 } // TODO: choose a better see
{
	alias_scale_ = alias_.probabilities.size();
}

void Worker::start() {
	// Use a lambda to ensure the std::stop_token is forwarded correctly to the member function
	thread_ = std::jthread([this](std::stop_token stoken) { this->run(stoken); });
}

void Worker::run(std::stop_token stoken) {
	// it doesn't rlly matter where we start at (see next lines)
	// for now our point starts with -1 <= x, y <= 1
	float x{ generator_() * 2.0f - 1.0f };
	float y{ generator_() * 2.0f - 1.0f };
	float color{ static_cast<float>(generator_()) };

	// our random initialization of the point means that the first few
	// iterations will quickly converge to some pattern, but this should
	// not be included in the final drawing (b/c it's ugly).
	// So I set the point in motion without accumulating color to settle
	// into some converged state
	for (int i{ 0 }; i < 20; ++i) {
		const Transformation& transformation{
			chooseRandomTransformation(alias_, generator_, alias_scale_, flame_) };

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

	while (!stoken.stop_requested()) {
		for (int i{ 0 }; i < config_.iterations_; ++i) {
			const Transformation& t{
				chooseRandomTransformation(alias_, generator_, alias_scale_, flame_) };

			float x_affine{ t.factors_.a_ * x + t.factors_.b_ * y + t.factors_.c_ };
			float y_affine{ t.factors_.d_ * x + t.factors_.e_ * y + t.factors_.f_ };

			Point2D updated_point{ applyVariations(t.variations_, x_affine, y_affine) };
			x = updated_point.x_;
			y = updated_point.y_;

			color = (color + t.color_) * 0.5f;
			buffer_.incrementFrequency(x, y, color);
		}

		flush_callback_(buffer_, config_.iterations_);
		buffer_.clear();
	}
}
