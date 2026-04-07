#pragma once

#include "robin/color/color.h"
#include "robin/color/gradient_stop.h"
#include "robin/constants.h"

#include <array>
#include <vector>

class GradientLookup {
public:
	GradientLookup() = delete;

	/**
	* Generates a highly performant gradient lookup table given various
	* intermediate color points
	* 
	* @param gradient_points Vector of gradient stop points
	*/
	GradientLookup(std::vector<GradientStop>& gradient_points);

	Color sampleGradientColor(float position);

private:
	std::array<Color, constants::kGradientLookupSize> lookup_table_{};
};
