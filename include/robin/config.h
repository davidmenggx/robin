#pragma once

#include "constants.h"

#include <string>

struct Config {
	std::string output_name_{ "output" };

	long long iterations_{ constants::kIterationsPerUpdate };

	int gui_width_{ constants::kWidth };
	int gui_height_{ constants::kHeight };

	bool show_telemetry_{};
	bool fullscreen_{};
};
