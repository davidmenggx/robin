#include "robin/color/gradient_stop.h"
#include "robin/config.h"
#include "robin/generation/affine.h"
#include "robin/generation/transformation.h"
#include "robin/generation/variation.h"
#include "robin/utils/parse_flame_options.h"

#include <format>
#include <fstream>
#include <stdexcept>
#include <vector>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

static std::vector<GradientStop> extractGradient(json data) {
	std::vector<GradientStop> gradient{};

	for (const auto& item : data["gradient"]) {
		GradientStop point{};

		point.position_ = item.value("pos", 0.0f);
		point.color_.red_ = item.value("r", 0);
		point.color_.green_ = item.value("g", 0);
		point.color_.blue_ = item.value("b", 0);

		gradient.push_back(point);
	}

	return gradient;
}

static std::vector<Transformation> extractTransformations(json data) {
	std::vector<Transformation> transformations{};

	for (const auto& item : data["transformations"]) {
		Transformation transform{};

		transform.weight_ = item.value("weight", 0.0f);
		transform.color_ = item.value("color", 0.0f);
		
		if (item.contains("affine")) {
			const auto& affine{ item["affine"] };
			transform.factors_.a_ = affine.value("a", 0.0f);
			transform.factors_.b_ = affine.value("b", 0.0f);
			transform.factors_.c_ = affine.value("c", 0.0f);
			transform.factors_.d_ = affine.value("d", 0.0f);
			transform.factors_.e_ = affine.value("e", 0.0f);
			transform.factors_.f_ = affine.value("f", 0.0f);
		}
		
		if (item.contains("variations") && item["variations"].is_array()) {
			std::vector<Variation> variations{};
			
			for (const auto& var : item["variations"]) {
				Variation variation{};

				variation.type_ = getVariationType(var["type"]);
				variation.weight_ = var.value("weight", 0.0f);

				variations.push_back(variation);
			}

			transform.variations_ = variations;
		}

		transformations.push_back(transform);
	}

	return transformations;
}

void utils::parseSettings(Config& config) {
	std::ifstream file(config.input_name_ + ".json");

	if (!file.is_open()) {
		throw std::invalid_argument{ std::format("Could not open input JSON data from file: {}", config.input_name_) };
	}

	json data{};
	try {
		file >> data;
	}
	catch (json::parse_error& e) {
		// this is a bit sketchy (catch then rethrow)
		// i guess it encapsulates the json library to just this file
		throw std::runtime_error{ e.what() };
	}

	if (data.contains("gradient") && data["gradient"].is_array()) {
		config.gradient_ = extractGradient(data);
	}

	std::vector<Transformation> transformations{};
	if (data.contains("transformations") && data["transformations"].is_array()) {
		config.transformation_ = extractTransformations(data);
	}
}
