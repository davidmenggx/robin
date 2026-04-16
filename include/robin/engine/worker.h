#pragma once

#include "robin/color/accumulation.h"
#include "robin/config.h"
#include "robin/distribution/alias_distribution.h"
#include "robin/distribution/xoshiro.h"
#include "robin/generation/flame.h"

#include <atomic>
#include <cstdint>
#include <stop_token>
#include <thread>

class Worker {
public:
	Worker(const Flame& flame, AliasTable& alias, Config& config, 
		Accumulation& buffer, std::atomic<uint64_t>& total_points);

	void start();

private:
	void run(std::stop_token stoken);

	// unfortunately for padding the config needs to be initialized rather
	// early so that we don't get weird seg faults in the constructor
	Config& config_;

	Accumulation& buffer_;

	AliasTable& alias_;

	Xoshiro256 generator_;

	const Flame& flame_;

	float alias_scale_{};

	std::atomic<uint64_t>& total_points_;

	// IMPORTANT: the thread must be at the end so the destructor order is preserved
	std::jthread thread_{};
};
