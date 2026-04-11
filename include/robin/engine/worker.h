#pragma once

#include "robin/color/accumulation.h"
#include "robin/config.h"
#include "robin/engine/cdf.h"
#include "robin/generation/flame.h"

#include <cstdint>
#include <functional>
#include <random>
#include <stop_token>
#include <thread>

class Worker {
public:
	Worker(const Flame& flame, AliasTable& alias, Config& config, 
		std::function<void(const Accumulation&, uint64_t)> flush_callback);

	void start();

private:
	void run(std::stop_token stoken);

	const Flame& flame_;
	AliasTable& alias_;
	Config& config_;

	Accumulation buffer_;

	std::mt19937 generator_;
	std::uniform_real_distribution<float> distribution_;

	std::function<void(const Accumulation&, uint64_t)> flush_callback_{};

	// IMPORTANT: the thread must be at the end so the destructor order is preserved
	std::jthread thread_{};
};
