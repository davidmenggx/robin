#include "color/accumulation.h"
#include "constants.h"
#include "engine/cdf.h"
#include "engine/engine.h"
#include "generation/flame.h"
#include "generation/transformation.h"
#include "generation/variation.h"

#include <iostream>
#include <random>
#include <vector>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

using namespace ff;

int main(int argc, char** argv) { 
  // some hard coded transformations for initial testing
  Transformation t1{{0.5f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f},
                    1.0f,
                    0.0f,
                    {{VariationType::kLinear, 1.0f}}};

  Transformation t2{{0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f},
                    1.0f,
                    0.5f,
                    {{VariationType::kLinear, 1.0f}}};

  Transformation t3{{0.5f, 0.0f, 0.25f, 0.0f, 0.5f, 0.5f},
                    1.0f,
                    1.0f,
                    {{VariationType::kLinear, 1.0f}}};

  std::vector<Transformation> test_transformations{{t1, t2, t3}};

  Flame flame{test_transformations};

  std::vector<float> cdf{GenerateCDF(test_transformations)};

  Accumulation buffer{};

  std::random_device device{};
  std::mt19937 generator(device());
  std::uniform_real_distribution<float> distribution{0, 1};

  std::cout << "starting\n";

  Iterate(flame, cdf, buffer, generator, distribution, constants::kIterations);

  std::cout << "done\n";

  // TODO: create some type of output for us to verify the results

  return 0;
}
