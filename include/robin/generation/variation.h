#pragma once

namespace ff {
	enum class VariationType {
		kLinear,
		kSinusoidal,
		kSpherical,
		kSwirl,
		kHorseshoe,
		// TODO: add more
	};

	struct Variation {
		VariationType type_{};
		float weight_{}; // how much this given variation contributes to the final output point
	};

	void applyVariation(VariationType type, float x_in, float y_in, float& x_out, float& y_out,
		float radius = 0, float theta = 0, float phi = 0);
}
