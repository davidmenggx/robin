#include "color/accumulation.h"
#include "constants.h"
#include "engine/cdf.h"
#include "engine/engine.h"
#include "generation/flame.h"
#include "generation/transformation.h"
#include "generation/variation.h"

#include <algorithm>
#include <cstdint>
#include <format>
#include <iostream>
#include <random>
#include <vector>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

using namespace ff;

int main(int argc, char** argv) {
	// some hard coded transformations for initial testing
	// this is the sierpinski triangle
	Transformation t1{ {0.5f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f},
					  1.0f,
					  0.0f,
					  {{VariationType::kLinear, 1.0f}} };

	Transformation t2{ {0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f},
					  1.0f,
					  0.5f,
					  {{VariationType::kLinear, 1.0f}} };

	Transformation t3{ {0.5f, 0.0f, 0.25f, 0.0f, 0.5f, 0.5f},
					  1.0f,
					  1.0f,
					  {{VariationType::kLinear, 1.0f}} };

	std::vector<Transformation> test_transformations{ {t1, t2, t3} };

	Flame flame{ test_transformations };

	std::vector<float> cdf{ GenerateCDF(test_transformations) };

	Accumulation buffer{};

	std::random_device device{};
	std::mt19937 generator(device());
	std::uniform_real_distribution<float> distribution{ 0, 1 };

	std::cout << "starting\n";

	Iterate(flame, cdf, buffer, generator, distribution, constants::kIterations);

	std::cout << "done\n";

	// DEBUG OUTPUTS:

	// Make sure the histogram is ok
	int total_hits{};
	int max_density{};
	int zero_pixels{};

	for (auto& pixel : buffer.histogram_) {
		total_hits += pixel.frequency_;
		max_density = std::max(max_density, pixel.frequency_);
		if (pixel.frequency_ == 0) {
			++zero_pixels;
		}
	}

	int nonzero_pixels{ static_cast<int>(buffer.histogram_.size() - zero_pixels) };
	std::cout << std::format("Total hits: {}\nMax density: {}\nZero pixels: {}\n",
		total_hits, max_density, zero_pixels);

	// Make sure the colors are ok
	float min_red{ 1.0f };
	float max_red{};

	float min_green{ 1.0f };
	float max_green{};

	float min_blue{ 1.0f };
	float max_blue{};

	int invalid_color{};

	for (auto& pixel : buffer.histogram_) {
		if (pixel.frequency_ == 0) {
			continue;
		}
		if (pixel.red_ < 0.0f || pixel.red_ > 1.0f
			|| pixel.green_ < 0.0f || pixel.green_ > 1.0f
			|| pixel.blue_ < 0.0f || pixel.blue_ > 1.0f
			) {
			++invalid_color;
		}
		min_red = std::min(min_red, pixel.red_);
		max_red = std::max(max_red, pixel.red_);

		min_green = std::min(min_green, pixel.green_);
		max_green = std::max(max_green, pixel.green_);

		min_blue = std::min(min_blue, pixel.blue_);
		max_blue = std::max(max_blue, pixel.blue_);
	}

	std::cout << std::format(
		"Invalid colors: {}\nReds: [{}, {}]\nGreens: [{}, {}]\nBlues: [{}, "
		"{}]\n",
		invalid_color,
		min_red, max_red,
		min_green, max_green,
		min_blue, max_blue);

	return 0;
}
