#pragma once

#include "constants.h"

#include <array>

namespace ff {
    struct TelemetryHUD {
        std::array<float, constants::kTelemetryHistorySize> samples_{};
        int head_{};
        float max_points_per_second_{ 1.0f };

        void Push(float points_per_second);
    };
}
