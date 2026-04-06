#pragma once

#include "robin/config.h"

#include <vector>

namespace utils {
	// TODO: update this throws list
	/**
	* @throws std::invalid_argument if file not found
	* @throws std::runtime_error if json not able to be parsed
	*/
	void parseSettings(Config& config);
}
