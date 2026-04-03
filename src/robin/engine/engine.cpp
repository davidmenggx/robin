#include "color/accumulation.h"
#include "engine/cdf.h"
#include "engine/engine.h"
#include "generation/flame.h"
#include "generation/variation.h"

#include <cmath>
#include <cstdint>
#include <random>
#include <tuple>

using namespace ff;

const Transformation& ff::ChooseTransformation(
    const std::vector<float>& cdf,
    std::mt19937& generator, 
    std::uniform_real_distribution<float>& distribution,
    const Flame& flame
    ) {
  float random{distribution(generator)};
  std::size_t transformation_idx{DrawCDF(cdf, random)};
  return flame.transformations_[transformation_idx];
}

void ff::ApplyVariations(
    const std::vector<Variation>& variations, 
    float x_in, float y_in, float& x_out, float& y_out
    ) {
  float radius{std::sqrt(x_in * x_in + y_in * y_in)};
  float theta{std::atan2(y_in, x_in)};
  float phi{std::atan2(x_in, y_in)};

  float x_accumulated{}, y_accumulated{};

  for (const Variation& variation : variations) {
    float x_var{}, y_var{};
    ApplyVariation(variation.type_, x_in, y_in, radius, theta, phi, x_var, y_var);
    x_accumulated += variation.weight_ * x_var;
    y_accumulated += variation.weight_ * y_var;
  }

  x_out = x_accumulated;
  y_out = y_accumulated;
}

void ff::Iterate(const Flame& flame,
	const std::vector<float>& cdf,
	Accumulation& buffer,
	std::mt19937& generator,
	std::uniform_real_distribution<float>& distribution,
	int iterations
	) {
  // it doesn't rlly matter where we start at (see next lines)
  // for now our point starts with -1 <= x, y <= 1
  float x{distribution(generator) * 2.0f - 1.0f};
  float y{distribution(generator) * 2.0f - 1.0f};
  float color{distribution(generator)};

  // our random initialization of the point means that the first few
  // iterations will quickly converge to some pattern, but this should
  // not be included in the final drawing (b/c it's ugly).
  // So I set the point in motion without accumulating color to settle
  // into some converged state
  for (int i{0}; i < 20; ++i) {
    const Transformation& transformation{
        ChooseTransformation(cdf, generator, distribution, flame)};

	// apply it
	// matrix multiplication:
    float x_affine = transformation.factors_.a_ * x +
                  transformation.factors_.b_ * y + transformation.factors_.c_;
    float y_affine = transformation.factors_.d_ * x +
                  transformation.factors_.e_ * y + transformation.factors_.f_;

    ApplyVariations(transformation.variations_, x_affine, y_affine, x, y);

	color = (color + transformation.color_) * 0.5f;
  }

  for (int i{0}; i < iterations; ++i) {
    const Transformation& transformation{
        ChooseTransformation(cdf, generator, distribution, flame)};

	float x_affine = transformation.factors_.a_ * x +
                     transformation.factors_.b_ * y + transformation.factors_.c_;
    float y_affine = transformation.factors_.d_ * x +
                     transformation.factors_.e_ * y + transformation.factors_.f_;

    ApplyVariations(transformation.variations_, x_affine, y_affine, x, y);

    color = (color + transformation.color_) * 0.5f;

    buffer.Accumulate(x, y, color);
  }
}
