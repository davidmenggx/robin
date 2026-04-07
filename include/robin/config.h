#pragma once

#include "constants.h"
#include "robin/color/color.h"
#include "robin/color/gradient_stop.h"
#include "robin/generation/transformation.h"

#include <string>
#include <vector>

struct Config {
	std::vector<Transformation> transformation_{ constants::kDefaultTransformation };
	std::vector<GradientStop> gradient_{ constants::kDefaultGradientStops };

	Color background_{ constants::kBackground };

	std::string input_name_{ constants::kInputFilename };
	std::string output_name_{ constants::kOutputFilename };

	long long iterations_{ constants::kIterationsPerUpdate };

	int gui_width_{ constants::kWidth };
	int gui_height_{ constants::kHeight };

	bool show_telemetry_{};
	bool fullscreen_{};
};
