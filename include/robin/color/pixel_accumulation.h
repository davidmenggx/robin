#pragma once

#include <cstdint>

namespace ff {
  struct PixelAccumulation {
    int frequency_{};
    float red_{};
    float green_{};
    float blue_{};
  };
}
