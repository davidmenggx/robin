#include "robin/color/accumulation.h"
#include "robin/config.h"
#include "robin/engine/cdf.h"
#include "robin/engine/engine.h"
#include "robin/engine/worker.h"
#include "robin/render/frame_events.h"
#include "robin/render/tonemap.h"
#include "robin/utils/save_image.h"

#include <chrono>
#include <cstdint>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>

Engine::Engine(Config& config)
	: config_{ config }, master_buffer_{ config }, renderer_{ "robin", config }
	, flame_{ config.transformation_ }, alias_{ generateDistribution(config.transformation_) }
{
}

void Engine::run() {
	unsigned int num_threads{ std::thread::hardware_concurrency() - 1 };
	if (num_threads == 0) {
		num_threads = 4;
	}

	for (unsigned int i{ 0 }; i < num_threads; ++i) {
		std::function<void(const Accumulation&, uint64_t)> callback = [this](const Accumulation& local_buffer, uint64_t points_processed) {
			this->flushWorkerBuffer(local_buffer, points_processed);
			};

		workers_.push_back(std::make_unique<Worker>(flame_, alias_, config_, callback));
		workers_.back()->start();
	}

	int frame{ 0 };

	auto last_telemetry_time = std::chrono::steady_clock::now();
	uint64_t last_telemetry_points{ total_points_.load() };

	auto last_frame_time = std::chrono::steady_clock::now();

	while (true) {
		FrameEvents events{ renderer_.fetchUserInput() };
		if (events.quit_) break;

		if (events.save_) {
			std::lock_guard<std::mutex> lock(master_mutex_);
			if (!utils::saveImage(generateTonemap(master_buffer_, config_.gui_width_, config_.gui_height_), config_)) {
				std::cerr << "Failed to save image output\n";
			}
		}

		auto current_time = std::chrono::steady_clock::now();
		auto time_since_last_frame = current_time - last_frame_time;

		// update output around 60 fps
		if (time_since_last_frame >= std::chrono::milliseconds(16)) {
			float elapsed_seconds{ std::chrono::duration<float>(current_time - last_telemetry_time).count() };

			if (elapsed_seconds >= 0.25f) {
				uint64_t current_points{ total_points_.load() };
				uint64_t points_processed{ current_points - last_telemetry_points };
				double points_per_second{ static_cast<double>(points_processed) / elapsed_seconds };

				renderer_.updateTelemetry(current_points, points_per_second);

				last_telemetry_time = current_time;
				last_telemetry_points = current_points;
			}

			{
				std::lock_guard<std::mutex> lock(master_mutex_);
				renderer_.updateGUI(master_buffer_);
			}

			last_frame_time = current_time;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	// cleanup (only works bc i use jthread)
	workers_.clear();
}

void Engine::flushWorkerBuffer(const Accumulation& local_buffer, uint64_t points_processed) {
	std::lock_guard<std::mutex> lock(master_mutex_);
	master_buffer_.merge(local_buffer);
	total_points_ += points_processed;
}
