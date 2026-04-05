#include "constants.h"
#include "render/telemetry_hud.h"

#include <algorithm>

using namespace ff;

void ff::TelemetryHUD::push(float points_per_second) {
    samples_[head_] = points_per_second;
    head_ = (head_ + 1) % constants::kTelemetryHistorySize;
    max_points_per_second_ = *std::max_element(samples_.begin(), samples_.end());
}
