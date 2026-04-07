#pragma once

#include "robin/color/color.h"
#include "robin/config.h"

#include <vector>

namespace utils {
	bool saveImage(const std::vector<Color>& buffer, Config& config);
}
