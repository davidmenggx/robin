#pragma once

#include "robin/color/accumulation.h"
#include "robin/config.h"
#include "robin/distribution/alias_distribution.h"
#include "robin/distribution/xoshiro.h"
#include "robin/generation/flame.h"

#include <cstdint>
#include <functional>
#include <stop_token>
#include <thread>

class Worker {
public:
	Worker(const Flame& flame, AliasTable& alias, Config& config, 
		std::function<void(const Accumulation&, uint64_t)> flush_callback);

	void start();

private:
	void run(std::stop_token stoken);

	// unfortunately for padding the config needs to be initialized rather
	// early so that we don't get weird seg faults in the constructor
	Config& config_;

	Accumulation buffer_;

	AliasTable& alias_;

	std::function<void(const Accumulation&, uint64_t)> flush_callback_{};

	Xoshiro256 generator_;

	const Flame& flame_;

	float alias_scale_{};

	// IMPORTANT: the thread must be at the end so the destructor order is preserved
	std::jthread thread_{};
};
