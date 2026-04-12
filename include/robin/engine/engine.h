#pragma once

#include "robin/color/accumulation.h"
#include "robin/config.h"
#include "robin/distribution/alias_distribution.h"
#include "robin/engine/worker.h"
#include "robin/generation/flame.h"
#include "robin/render/renderer.h"

#include <atomic>
#include <cstdint>
#include <memory>
#include <mutex>
#include <vector>

class Engine {
public:
	Engine(Config& config);

	void run();

private:
	void flushWorkerBuffer(const Accumulation& local_buffer, uint64_t points_processed);

	Accumulation master_buffer_;

	Renderer renderer_;

	Config& config_;

	std::mutex master_mutex_{};

	AliasTable alias_{};
	Flame flame_{};

	std::vector<std::unique_ptr<Worker>> workers_{};

	std::atomic<uint64_t> total_points_{};
};
