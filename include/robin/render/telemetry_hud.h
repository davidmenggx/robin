#pragma once

#include "robin/constants.h"

#include <array>

struct TelemetryHUD {
	std::array<float, constants::kTelemetryHistorySize> samples_{};
	int head_{};
	float max_points_per_second_{ 1.0f };

	void updatePointsHistory(float points_per_second);
};
