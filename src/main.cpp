#include "color/accumulation.h"
#include "color/pixel_accumulation.h"
#include "constants.h"
#include "engine/cdf.h"
#include "engine/engine.h"
#include "generation/flame.h"
#include "generation/transformation.h"
#include "generation/variation.h"

#include <algorithm>
#include <cmath>
#include <format>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

using namespace ff;

void LogRenderingOutput(Accumulation& buffer) {
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
}

void GeneratePPM(const std::string& output_path, Accumulation& buffer) {
	std::ofstream outf{ output_path };
	if (!outf) {
		std::cout << std::format("Invalid output path. aborting: {}\n", output_path);
		return;
	}

	outf << "P3\n";
	outf << constants::kWidth << ' ' << constants::kHeight << '\n';
	outf << 255 << '\n';

	int max_density{};
	for (auto& pixel : buffer.histogram_) {
		max_density = std::max(max_density, pixel.frequency_);
	}

	// tone mapping:
	for (int y{}; y < constants::kHeight; ++y) {
		for (int x{}; x < constants::kWidth; ++x) {
			PixelAccumulation& pixel = buffer.Get(x, y);
			if (pixel.frequency_ == 0) {
				outf << "0 0 0 ";
			}
			else {
				// log-normalized alpha
				double alpha{ std::log(pixel.frequency_ + 1.0) / std::log(max_density + 1.0) };

				// apply alpha and gamma, then scale to [0, 255]
				double gamma{ 2.2 };
				auto to_byte = [&](float channel) -> int {
					double v = channel * alpha;
					v = std::pow(v, 1.0 / gamma);
					return static_cast<int>(std::clamp(v * 255.0, 0.0, 255.0));
					};

				outf << std::format("{} {} {} ",
					to_byte(pixel.red_),
					to_byte(pixel.green_),
					to_byte(pixel.blue_));
			}
		}
		outf << '\n';
	}
}

int main(int argc, char** argv) {
	// some hard coded transformations for initial testing
	// this is the sierpinski triangle
	Transformation t1{ {0.5f, 0.0f, 0.0f,
						0.0f, 0.5f, 0.0f},
					  1.0f,
					  0.0f,
					  {{VariationType::kLinear, 1.0f}} };

	Transformation t2{ {0.5f, 0.0f, 0.5f,
						0.0f, 0.5f, 0.0f},
					  1.0f,
					  0.5f,
					  {{VariationType::kLinear, 1.0f}} };

	Transformation t3{ {0.5f, 0.0f, 0.25f,
						0.0f, 0.5f, 0.43f},
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

	LogRenderingOutput(buffer);

	// TEMPORARY IMAGE OUTPUT FOR DEBUGGING PURPOSES:

	GeneratePPM("test_output.ppm", buffer);

	return 0;
}
