#pragma once

#include "robin/color/accumulation.h"
#include "robin/config.h"
#include "robin/engine/worker.h"
#include "robin/generation/flame.h"
#include "robin/render/renderer.h"

#include <atomic>
#include <chrono>
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

	Config& config_;
	Flame flame_{};
	std::vector<float> cdf_{};

	Accumulation master_buffer_;
	std::mutex master_mutex_{};

	std::vector<std::unique_ptr<Worker>> workers_{};

	std::atomic<uint64_t> total_points_{};

	Renderer renderer_;
};
