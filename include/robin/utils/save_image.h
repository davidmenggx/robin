#pragma once

#include "robin/color/color.h"
#include "robin/config.h"
#include "robin/constants.h"

#include <string>
#include <vector>

namespace utils {
	bool saveImage(const std::vector<ff::Color>& buffer, const Config& config);
}
