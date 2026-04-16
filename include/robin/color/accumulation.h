#pragma once

#include "robin/color/gradient_lookup.h"
#include "robin/config.h"

#include <atomic>
#include <cstdint>
#include <utility>
#include <vector>

class Accumulation {
public:
	explicit Accumulation(Config& config);

	[[nodiscard]] uint64_t getMaxColorFrequency() const;

	void markPixelHit(float x, float y, float color);

	// TODO: make these private again after initial testing
	// MAJOR CHANGE: I use a structure of arrays approach instead to enable atomics
	std::vector<std::atomic<uint64_t>> frequency_{};
	std::vector<std::atomic<uint64_t>> red_{};
	std::vector<std::atomic<uint64_t>> green_{};
	std::vector<std::atomic<uint64_t>> blue_{};

private:
	std::pair<int, int> projectToScreen(float x, float y);

	// unfortunately for padding the config needs to be initialized rather
	// early so that we don't get weird seg faults in the constructor
	Config& config_;

	// a lookup table for sampling continuous gradient RGB tuples, generated
	// from a finite set of gradient stop points
	GradientLookup gradient_lookup_;
};
