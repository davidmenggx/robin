#include "generation/variation.h"

#include <cmath>
#include <stdexcept>
#include <utility>

using namespace ff;

void ff::ApplyVariation(
    VariationType type, float x_in, float y_in,
    float radius, float theta, float phi,
    float& x_out, float& y_out
    ) {
  float radius_squared = radius * radius;
  // TODO: maybe think of a better way to do this long term
  switch (type) { 
  case VariationType::kLinear:
    x_out = x_in;
    y_out = y_in;
    break;

  case VariationType::kSinusoidal:
    x_out = std::sin(x_in);
    y_out = std::sin(y_in);
    break;

  case VariationType::kSpherical:
    x_out = x_in / radius_squared;
    y_out = y_in / radius_squared;
    break;

  case VariationType::kSwirl:
    x_out = x_in * std::sin(radius_squared) - y_in * std::cos(radius_squared);
    y_out = x_in * std::cos(radius_squared) - y_in * std::sin(radius_squared);
    break;

  case VariationType::kHorseshoe:
    if (radius == 0.0f) {
      x_out = 0.0f;
      y_out = 0.0f;
      break;
    }
    x_out = ((x_in - y_in) * (x_in + y_in)) / radius;
    y_out = (2.0f * x_in * y_in) / radius;
    break;
  
  default:
    throw std::runtime_error("Unknown variation type");
  }
}
