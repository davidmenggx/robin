#include "color/color.h"
#include "color/gradient_lookup.h"
#include "color/gradient_point.h"

#include <algorithm>

using namespace ff;

GradientLookup::GradientLookup(std::vector<GradientPoint>& gradient_points) {
  for (int i{0}; i < constants::kGradientLookupSize; ++i) {
    float transition_position{static_cast<float>(i) / (constants::kGradientLookupSize - 1)};

    GradientPoint lower_bound{gradient_points.front()};
    GradientPoint upper_bound{gradient_points.back()};
    for (std::size_t j{0}; j < std::size(gradient_points); ++j) {
      if (transition_position >= gradient_points[j].position_ &&
          transition_position <= gradient_points[j + 1].position_) {
        lower_bound = gradient_points[j];
        upper_bound = gradient_points[j + 1];
        break;
      }
    }

    float transition_range{upper_bound.position_ - lower_bound.position_};
    
    float blend{};
    if (transition_range > 0) {
      blend = (transition_position - lower_bound.position_) / transition_range;
    } else {
      blend = 0.0f;
    }

    lookup_table_[i] = {
        static_cast<uint8_t>(
            lower_bound.color_.red_ +
            blend * (static_cast<int>(upper_bound.color_.red_) - 
                static_cast<int>(lower_bound.color_.red_))),
        static_cast<uint8_t>(
            lower_bound.color_.green_ +
            blend * (static_cast<int>(upper_bound.color_.green_) - 
                static_cast<int>(lower_bound.color_.green_))),
        static_cast<uint8_t>(
            lower_bound.color_.blue_ +
            blend * (static_cast<int>(upper_bound.color_.blue_) -
                static_cast<int>(lower_bound.color_.blue_)))
    };
  }
}

Color GradientLookup::Sample(float color) {
  int index{static_cast<int>(color * (constants::kGradientLookupSize - 1))};
  index = std::clamp(index, 0, constants::kGradientLookupSize - 1);
  return lookup_table_[index];
}
