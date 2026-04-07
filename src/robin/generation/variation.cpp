#include "robin/generation/point2d.h"
#include "robin/generation/variation.h"

#include <cmath>
#include <stdexcept>
#include <string>
#include <unordered_map>

Point2D applyVariation(
	VariationType type, 
	float x, float y,
	float radius, float theta, float phi) {
	float radius_squared = radius * radius;
	// TODO: maybe think of a better way to do this long term
	switch (type) {
	case VariationType::kLinear:
		return { x, y };
	case VariationType::kSinusoidal:
		return { std::sin(x), std::sin(y) };
	case VariationType::kSpherical:
		return { x / radius_squared, y / radius_squared };
	case VariationType::kSwirl:
		return {
			x * std::sin(radius_squared) - y * std::cos(radius_squared),
			x * std::cos(radius_squared) - y * std::sin(radius_squared)
		};
	case VariationType::kHorseshoe:
		if (radius == 0.0f) {
			return { 0.0f, 0.0f };
		}
		return {
			((x - y) * (x + y)) / radius,
			(2.0f * x * y) / radius
		};
	default:
		throw std::domain_error("Unknown variation type");
	}
}

VariationType getVariationType(const std::string& type) {
	static const std::unordered_map<std::string, VariationType> lookup{
		{
			{"linear", VariationType::kLinear},
			{"sinusoidal", VariationType::kSinusoidal},
			{"spherical", VariationType::kSpherical},
			{"swirl", VariationType::kSwirl},
			{"horseshoe", VariationType::kHorseshoe},
		}
	};

	auto it = lookup.find(type);
	if (it != lookup.end()) {
		return it->second;
	}

	throw std::domain_error("Invalid variation type");
}
