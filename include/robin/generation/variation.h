#pragma once

#include <string>

namespace ff {
	enum class VariationType {
		kLinear,
		kSinusoidal,
		kSpherical,
		kSwirl,
		kHorseshoe,
		// TODO: add more
	};

	/**
	* @throws std::runtime_error if type is unknown
	*/
	VariationType getType(const std::string& type);

	struct Variation {
		VariationType type_{};
		float weight_{}; // how much this given variation contributes to the final output point
	};

	/**
	* @throws std::runtime_error if type is unknown
	*/
	void applyVariation(VariationType type, float x_in, float y_in, float& x_out, float& y_out,
		float radius = 0, float theta = 0, float phi = 0);
}
