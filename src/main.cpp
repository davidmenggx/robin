#include "robin/config.h"
#include "robin/engine/engine.h"
#include "robin/generation/transformation.h"
#include "robin/generation/variation.h"
#include "robin/utils/parse_settings.h"

#include <exception>
#include <vector>

#include <CLI/App.hpp>

using namespace ff;

int main(int argc, char** argv) {
	CLI::App app{"High-performance fractal flame renderer", "robin"};

	Config config{};

	app.add_flag("-t,--telemetry", config.show_telemetry_, "Show performance statistics");
	app.add_flag("-f,--fullscreen", config.fullscreen_, "Render in full screen");

	app.add_option("--width", config.gui_width_, "Renderer width");
	app.add_option("--height", config.gui_height_, "Renderer height");
	app.add_option("-u,--iterations-per-update", config.iterations_, "Number of iterations per render update");
	
	app.add_option("-i,--input", config.input_name_, "Input filename for transformations (.json)");
	app.add_option("-o,--output", config.output_name_, "Output filename for saved flame (.png). [s] to save");

	CLI11_PARSE(app, argc, argv);

	// TODO: move these guys to constants
	// some hard coded transformations for initial testing
	Transformation t1{ { 0.75f,  0.15f, 0.00f,
						 -0.15f,  0.75f, 0.00f},
						 1.0f,
						 0.0f,
						 {{VariationType::kSpherical, 0.6f}, {VariationType::kLinear, 0.4f}} };

	Transformation t2{ { 0.50f,  0.00f,  1.20f,
						 0.00f,  0.50f, -0.80f},
						 1.0f,
						 0.33f,
						 {{VariationType::kSinusoidal, 0.8f}, {VariationType::kLinear, 0.2f}} };

	Transformation t3{ { 0.40f, -0.40f, -1.00f,
						 0.40f,  0.40f,  0.50f},
						 1.0f,
						 0.66f,
						 {{VariationType::kHorseshoe, 0.5f}, {VariationType::kLinear, 0.5f}} };

	Transformation t4{ { 0.60f,  0.40f,  0.00f,
						-0.40f,  0.60f,  1.20f},
						 1.0f,
						 1.0f,
						 {{VariationType::kSwirl, 0.4f}, {VariationType::kLinear, 0.6f}} };

	try {
		utils::parseSettings(config);
	}
	catch (std::exception& e) {
		// TODO: figure out a better way to handle this
		// watch out bc runtime error can be thrown in multiple ways
		config.transformation_ = { t1, t2, t3, t4 };
	}

	runEngine(config);

	return 0;
}
