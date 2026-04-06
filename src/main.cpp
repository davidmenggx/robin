#include "color/accumulation.h"
#include "config.h"
#include "constants.h"
#include "engine/cdf.h"
#include "engine/engine.h"
#include "generation/flame.h"
#include "generation/transformation.h"
#include "render/renderer.h"

#include <chrono>
#include <random>
#include <ratio>
#include <vector>

#include <CLI/CLI.hpp>

using namespace ff;

void runEngine(std::vector<Transformation>& transformations, const Config& config) {
	Flame flame{ transformations };

	std::vector<float> cdf{ generateCDF(transformations) };

	Accumulation buffer{ config };

	std::random_device device{};
	std::mt19937 generator(device());
	std::uniform_real_distribution<float> distribution{ 0, 1 };

	Renderer renderer{ "robin", config };

	int frame{};
	int total_points{};
	while (renderer.pollEvents()) {
		auto t1 = std::chrono::steady_clock::now();

		iterate(flame, cdf, buffer, generator, distribution, config.iterations_);
		total_points += config.iterations_;

		auto t2 = std::chrono::steady_clock::now();
		float ms{ static_cast<float>(std::chrono::duration<float, std::milli>(t2 - t1).count()) };
		float points_per_second{ static_cast<float>(config.iterations_ / (ms / 1000.0)) };

		renderer.updateTelemetry(total_points, points_per_second);
		renderer.update(buffer);

		if (frame % 10 == 0) {
			renderer.update(buffer);
		}
		++frame;
	}
}

int main(int argc, char** argv) {
	CLI::App app{"High-performance fractal flame renderer", "robin"};

	Config config{};

	app.add_flag("-t,--telemetry", config.show_telemetry_, "Show performance statistics");
	app.add_flag("-f,--fullscreen", config.fullscreen_, "Render in full screen");

	app.add_option("--width", config.gui_width_, "Renderer width");
	app.add_option("--height", config.gui_height_, "Renderer height");
	app.add_option("-i,--iterations-per-update", config.iterations_, "Number of iterations per render update");
	
	app.add_option("-o,--output", config.output_path_, "Output filename for saved flame. [s] to save");

	CLI11_PARSE(app, argc, argv);

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

	std::vector<Transformation> test_transformations{ {t1, t2, t3, t4} };

	runEngine(test_transformations, config);

	return 0;
}
