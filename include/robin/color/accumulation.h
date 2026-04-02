#pragma once

#include "constants.h"
#include "color/pixel_accumulation.h"

#include <cstdint>
#include <vector>

namespace ff {
  class Accumulation {
   public:
    Accumulation();

    PixelAccumulation& Get(uint32_t row, uint32_t col);

    void Accumulate(uint32_t row, uint32_t col);
   private:
    std::vector<PixelAccumulation> histogram_{};
  };
}

