#include "color/accumulation.h"
#include "constants.h"
#include "engine/cdf.h"
#include "engine/engine.h"
#include "generation/flame.h"
#include "generation/transformation.h"
#include "generation/variation.h"
#include "render/renderer.h"

#include <chrono>
#include <iostream>
#include <random>
#include <ratio>
#include <vector>

using namespace ff;

int main(int argc, char** argv) {
	// some hard coded transformations for initial testing
	// a cooler transformation
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

	Flame flame{ test_transformations };

	std::vector<float> cdf{ GenerateCDF(test_transformations) };

	Accumulation buffer{};

	std::random_device device{};
	std::mt19937 generator(device());
	std::uniform_real_distribution<float> distribution{ 0, 1 };

	// TODO: make this a commnad line arg
	Renderer renderer{ "robin", true };

	std::cout << "starting\n";

	int frame{};
	int total_points{};
	while (renderer.PollEvents()) {
		auto t1 = std::chrono::steady_clock::now();

		Iterate(flame, cdf, buffer, generator, distribution, 
			constants::kIterationsPerUpdate);
		total_points += constants::kIterationsPerUpdate;
		
		auto t2 = std::chrono::steady_clock::now();
		float ms{ static_cast<float>(std::chrono::duration<double, std::milli>(t2 - t1).count()) };
		float points_per_second{ static_cast<float>(constants::kIterationsPerUpdate / (ms / 1000.0)) };

		renderer.UpdateTelemetry(total_points, points_per_second);
		renderer.Update(buffer);

		if (frame % 10 == 0) {
			renderer.Update(buffer);
		}
		++frame;
	}

	std::cout << "done\n";

	return 0;
}
