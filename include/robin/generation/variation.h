#pragma once

#include "robin/generation/point2d.h"

#include <string>

enum class VariationType {
	kLinear,
	kSinusoidal,
	kSpherical,
	kSwirl,
	kHorseshoe,
	kPolar,
	kHandkerchief,
	kHeart,
	kDisc,
	kSpiral,
	kHyperbolic,
	kDiamond,
	kEx,
	kBent,
	kFisheye,
	kExponential
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
Point2D applyVariation(VariationType type, float x, float y);
