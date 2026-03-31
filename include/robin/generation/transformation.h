#pragma once

#include "generation/affine.h"
#include "generation/variation.h"

#include <vector>

namespace ff {
  struct Transformation {
    Affine factors_{};
    double weight_{};  // probabiliy that this transformation is picked
    double color_{}; // 0.0 to 1.0 representing the index of a gradient
    // each transformation is the weighted sum of multiple variations
    std::vector<Variation> variations_{};
  };
}
