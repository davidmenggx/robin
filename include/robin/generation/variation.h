#pragma once

namespace ff {
  enum class VariationType { 
    kLinear,
    kSinusoidal,
    kSpherical,
    kSwirl,
    kHorseshoe,
    // TODO: add more
    kTotalVariations
  };

  struct Variation {
    VariationType type_{};
    float weight_{}; // how much this given variation contributes to the final output point
  };
}
