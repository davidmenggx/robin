#pragma once

#include "robin/color/accumulation.h"
#include "robin/color/color.h"

#include <vector>

std::vector<Color> generateTonemap(Accumulation& buffer, int width, int height);
