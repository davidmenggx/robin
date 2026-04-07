#pragma once

#include "robin/generation/point2d.h"

#include <string>

enum class VariationType {
	kLinear,
	kSinusoidal,
	kSpherical,
	kSwirl,
	kHorseshoe,
	// TODO: add more
};

/**
* @throws std::domain_error if type is unknown
*/
VariationType getVariationType(const std::string& type);

struct Variation {
	VariationType type_{};
	float weight_{}; // how much this given variation contributes to the final output point
};

/**
* Returns updated Point2D (x, y) after applying the specified transformation
* 
* @throws std::domain_error if type is unknown
*/
Point2D applyVariation(VariationType type, float x, float y,
	float radius = 0, float theta = 0, float phi = 0);
