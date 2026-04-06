#pragma once

#include "constants.h"
#include "robin/color/gradient_point.h"
#include "robin/generation/transformation.h"
#include "robin/render/color.h"

#include <string>
#include <vector>

struct Config {
	std::vector<ff::Transformation> transformation_{};
	std::vector<ff::GradientPoint> gradient_{ { ff::kGradientPoints } };

	render::Color background_{ constants::kBackground };

	std::string output_name_{ "output" };
	std::string input_name_{ "input" };

	long long iterations_{ constants::kIterationsPerUpdate };

	int gui_width_{ constants::kWidth };
	int gui_height_{ constants::kHeight };

	bool show_telemetry_{};
	bool fullscreen_{};
};
