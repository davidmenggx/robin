#include "robin/config.h"
#include "robin/engine/engine.h"
#include "robin/utils/parse_flame_options.h"

#include <exception>

#include <CLI/CLI.hpp>

int main(int argc, char** argv) {
	// parse command line inputs
	CLI::App app{"High-performance fractal flame renderer", "robin"};

	Config config{};

	// Display real-time performance metrics
	app.add_flag("-t,--telemetry", config.show_telemetry_, "Show performance statistics");

	// GUI options
	app.add_flag("-f,--fullscreen", config.fullscreen_, "Render in full screen");
	app.add_option("--width", config.gui_width_, "Renderer width");
	app.add_option("--height", config.gui_height_, "Renderer height");
	app.add_option("-u,--iterations-per-update", config.iterations_, "Number of iterations per render update");
	
	// I/O options
	app.add_option("-i,--input", config.input_name_, "Input filename for transformations (.json)");
	app.add_option("-o,--output", config.output_name_, "Output filename for saved flame (.png). [s] to save");

	CLI11_PARSE(app, argc, argv);

	try {
		utils::parseSettings(config);
	}
	catch (std::exception& e) {
		// TODO: error logging
	}

	runEngine(config);

	return 0;
}
