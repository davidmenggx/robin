#pragma once

#include "robin/config.h"

namespace utils {
	/**
	* Parses flame configuration json and loads into provided configuration
	* 
	* @throws std::invalid_argument if file not found
	* @throws std::runtime_error if json not able to be parsed
	* @throws std::domain_error if invalid variation type provided
	*/
	void parseSettings(Config& config);
}
