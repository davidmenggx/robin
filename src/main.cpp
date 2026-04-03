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

	Renderer renderer{ "robin" };

	std::cout << "starting\n";

	int frame{};
	int cumulative_points{};
	auto last_stat_update = std::chrono::steady_clock::now();
	int points_since_last_update{};

	while (renderer.PollEvents()) {
		auto t1 = std::chrono::steady_clock::now();

		Iterate(flame, cdf, buffer, generator, distribution, constants::kIterationsPerUpdate);
		
		if (frame % 10 == 0) {
			renderer.Update(buffer);
		}
		++frame;

		cumulative_points += constants::kIterationsPerUpdate;
		points_since_last_update += constants::kIterationsPerUpdate;

		auto now = std::chrono::steady_clock::now();
		double seconds_elapsed{ std::chrono::duration<double>(now - last_stat_update).count() };

		if (seconds_elapsed >= 0.5) {
			double points_per_second{ points_since_last_update / seconds_elapsed };

			renderer.UpdateStats(points_per_second, cumulative_points);

			points_since_last_update = 0;
			last_stat_update = now;
		}
	}

	std::cout << "done\n";

	return 0;
}
