#pragma once

#include "robin/generation/transformation.h"

#include <vector>

namespace ff {
	struct Flame {
		std::vector<Transformation> transformations_{};
	};
}
