#include "robin/generation/point2d.h"
#include "robin/generation/variation.h"

#include <cmath>
#include <numbers>
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
	case VariationType::kPolar:
		return { 
			static_cast<float>(theta / std::numbers::pi), 
			radius - 1.0f 
		};
	case VariationType::kHandkerchief:
		return {
			radius * std::sin(theta + radius),
			radius * std::cos(theta - radius)
		};
	case VariationType::kHeart:
		return {
			radius * std::sin(theta * radius),
			-radius * std::cos(theta * radius)
		};
	case VariationType::kDisc:
		return {
			static_cast<float>((theta / std::numbers::pi) * std::sin(std::numbers::pi * radius)),
			static_cast<float>((theta / std::numbers::pi)* std::cos(std::numbers::pi * radius))
		};
	case VariationType::kSpiral:
		if (radius == 0.0f) {
			return { 0.0f, 0.0f };
		}
		return {
			(1.0f / radius) * (std::cos(theta) + std::sin(radius)),
			(1.0f / radius) * (std::sin(theta) - std::cos(radius))
		};
	case VariationType::kHyperbolic:
		if (radius == 0.0f) {
			return { 0.0f, 0.0f };
		}
		return {
			std::sin(theta) / radius,
			radius * std::cos(theta)
		};
	case VariationType::kDiamond:
		return {
			std::sin(theta) * std::cos(radius),
			std::cos(theta) * std::sin(radius)
		};
	case VariationType::kEx: {
		float p0{ std::sin(theta + radius) };
		float p1{ std::cos(theta - radius) };
		return {
			radius * (p0 * p0 * p0 + p1 * p1 * p1),
			radius * (p0 * p0 * p0 - p1 * p1 * p1)
		};
	}
	case VariationType::kBent: {
		float out_x{ x };
		float out_y{ y };
		if (x < 0.0f) {
			out_x = 2.0f * x;
		}
		if (y < 0.0f) {
			out_y = y / 2.0f;
		}
		return { out_x, out_y };
	}
	case VariationType::kFisheye: {
		float factor{ 2.0f / (radius + 1.0f) };
		return { x * factor, y * factor };
	}
	case VariationType::kExponential: {
		float exp_val{ std::exp(x - 1.0f) };
		return {
			static_cast<float>(exp_val * std::cos(std::numbers::pi * y)),
			static_cast<float>(exp_val * std::sin(std::numbers::pi * y))
		};
	}
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
			{"polar", VariationType::kPolar},
			{"handkerchief", VariationType::kHandkerchief},
			{"heart", VariationType::kHeart},
			{"disc", VariationType::kDisc},
			{"spiral", VariationType::kSpiral},
			{"hyperbolic", VariationType::kHyperbolic},
			{"diamond", VariationType::kDiamond},
			{"ex", VariationType::kEx},
			{"bent", VariationType::kBent},
			{"fisheye", VariationType::kFisheye},
			{"exponential", VariationType::kExponential},
		}
	};

	auto it = lookup.find(type);
	if (it != lookup.end()) {
		return it->second;
	}

	throw std::domain_error("Invalid variation type");
}
