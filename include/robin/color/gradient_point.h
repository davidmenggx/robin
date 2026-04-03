#pragma once

#include "color/color.h"

#include <vector>

namespace ff {
  struct GradientPoint {
    float position_{}; // between 0 and 1 inclusive
    Color color_{};
  };

  // TEMPORARY GRADIENT POINTS, MOVE SOMEWHERE ELSE!!!!:
  inline std::vector<GradientPoint> kGradientPoints = {
      {0.0, 10, 0, 40},      // deep purple
      {0.3, 180, 0, 220},    // violet
      {0.6, 255, 100, 0},    // orange
      {1.0, 255, 255, 200},  // pale yellow
  };
}
