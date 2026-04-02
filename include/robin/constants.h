#pragma once

#include "render/color.h"

#include <cstdint>

namespace constants {
  // gui details
  inline constexpr int32_t kWidth{1000};
  inline constexpr int32_t kHeight{1000};
  inline constexpr render::Color kBackground{0, 0, 0};

  // TODO: replace this with some default command line arg
  inline constexpr uint64_t kIterations{1000};

  // Transformation factors
  // this represents how much the color value for the pixel
  // moves towards the transformation specified color value
  inline constexpr float kColorSpeed{0.5f};
  }
