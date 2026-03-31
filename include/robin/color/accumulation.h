#pragma once

#include "constants.h"
#include "color/pixel.h"

#include <cstdint>
#include <vector>

namespace ff {
  class Accumulation {
   public:
    Accumulation();

    ff::Pixel& get(int32_t row, int32_t col);

   private:
    std::vector<ff::Pixel> histogram_{};
  };
}

