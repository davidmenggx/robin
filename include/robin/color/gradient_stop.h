#pragma once

#include "robin/color/color.h"

struct GradientStop {
	float position_{}; // between 0 and 1 inclusive
	Color color_{};
};
