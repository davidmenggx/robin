#include "color/accumulation.h"
#include "constants.h"

#include <cstdint>

using namespace ff;

Accumulation::Accumulation() 
: histogram_(constants::kWidth * constants::kHeight) {}

[[nodiscard]] PixelAccumulation& Accumulation::Get(uint32_t row, uint32_t col) {
  return histogram_[row * constants::kWidth + col];
}

void Accumulation::Accumulate(uint32_t row, uint32_t col) {
  // TODO
}
