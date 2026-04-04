#pragma once

#include "render/color.h"

namespace constants {
	// gui details
	inline constexpr int kWidth{ 1500 };
	inline constexpr int kHeight{ 1500 };
	inline constexpr render::Color kBackground{ 0, 0, 0 };

	// runtime telemetry details
	inline constexpr int kTelemetryWidth{ 800 };
	inline constexpr int kTelemetryHeight{ 800 };
	inline constexpr int kTelemetryHistorySize{ 100 }; // how many samples saved
	inline constexpr int kTelemetryTicks{ 4 };

	// TODO: replace this with some default command line arg
	inline constexpr long long kIterationsPerUpdate{ 50'000ll };
	inline constexpr int kFramesPerUpdate{ 10 };

	// rendering settings:
	inline constexpr float kPixelsPerUnit{ 750.0f };
	inline constexpr int kGradientLookupSize{ 1024 };

	inline constexpr int kTelemetryMarginLeft{ 50 };
	inline constexpr int kTelemetryMarginRight{ 20 };
	inline constexpr int kTelemetryMarginTop{ 60 };
	inline constexpr int kTelemetryMarginBottom{ 30 };
	// prevent the line chart from maxing out at the top of the graph by
	// making the vertical axis extend higher than the true maximum
	inline constexpr float kTelemetryGraphUpperScaling{ 1.2f };
}
