#pragma once

#include "robin/color/color.h"
#include "robin/color/gradient_stop.h"
#include "robin/generation/transformation.h"
#include "robin/generation/variation.h"

#include <string>
#include <vector>

namespace constants {
	// gui constants
	inline constexpr int kWidth{ 1500 };
	inline constexpr int kHeight{ 1500 };
	inline constexpr Color kBackground{ 0, 0, 0, 255 };
	inline constexpr int kColorChannels{ 4 };

	// I/O constants
	inline const std::string kInputFilename{ "input" };
	inline const std::string kOutputFilename{ "output" };

	// runtime telemetry constants
	inline constexpr int kTelemetryWidth{ 800 };
	inline constexpr int kTelemetryHeight{ 800 };
	inline constexpr int kTelemetryHistorySize{ 100 }; // how many samples saved
	inline constexpr int kTelemetryTicks{ 4 };

	inline constexpr long long kIterationsPerUpdate{ 1'000'000ll };
	inline constexpr int kFramesPerUpdate{ 10 };

	// rendering constants:
	inline constexpr float kPixelsPerUnit{ 750.0f };
	// number of samples generated for gradient lookup table
	// aka the number the distinct color outputs possible
	inline constexpr int kGradientLookupSize{ 1024 };

	inline constexpr int kTelemetryMarginLeft{ 50 };
	inline constexpr int kTelemetryMarginRight{ 20 };
	inline constexpr int kTelemetryMarginTop{ 60 };
	inline constexpr int kTelemetryMarginBottom{ 30 };
	// prevent the line chart from maxing out at the top of the graph by
	// making the vertical axis extend higher than the true maximum
	inline constexpr float kTelemetryGraphUpperScaling{ 1.2f };

	// Default application settings:
	inline const std::vector<GradientStop> kDefaultGradientStops{ {
		{0.0f,   0,  15,  30},
		{0.35f, 10,  80, 160},
		{0.65f, 20, 220, 200},
		{1.0f, 220, 255, 255},
	} };

	inline const std::vector<Transformation> kDefaultTransformation{ 
		{
			// First transformation
			{{VariationType::kSpherical, 0.6f}, {VariationType::kLinear, 0.4f}},
			{ 0.75f,  0.15f, 0.00f, -0.15f,  0.75f, 0.00f}, 1.0f, 0.0f
		},
		{
			// Second transformation
			{{VariationType::kSinusoidal, 0.8f}, {VariationType::kLinear, 0.2f}},
			{ 0.50f,  0.00f,  1.20f, 0.00f,  0.50f, -0.80f}, 1.0f, 0.33f
		},
		{
			// Third transformation
			{{VariationType::kHorseshoe, 0.5f}, {VariationType::kLinear, 0.5f}},
			{ 0.40f, -0.40f, -1.00f, 0.40f,  0.40f,  0.50f}, 1.0f, 0.66f
		},
		{
			// Fourth transformation
			{{VariationType::kSwirl, 0.4f}, {VariationType::kLinear, 0.6f}},
			{ 0.60f,  0.40f,  0.00f, -0.40f,  0.60f,  1.20f}, 1.0f, 1.0f
		}
	};
}
