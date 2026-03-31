#include "color/accumulation.h"
#include "constants.h"

#include <cstdint>

using namespace ff;

Accumulation::Accumulation() 
: histogram_(constants::kWidth * constants::kHeight) {}

[[nodiscard]] Pixel& Accumulation::get(int32_t row, int32_t col) {
  return histogram_[row * constants::kWidth + col];
}
