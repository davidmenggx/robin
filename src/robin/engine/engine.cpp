#include "robin/color/accumulation.h"
#include "robin/config.h"
#include "robin/distribution/alias_distribution.h"
#include "robin/engine/engine.h"
#include "robin/engine/worker.h"
#include "robin/render/frame_events.h"
#include "robin/render/tonemap.h"
#include "robin/utils/save_image.h"

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <format>
#include <iostream>
#include <memory>
#include <random>
#include <shared_mutex>
#include <thread>

Engine::Engine(Config& config)
	: config_{ config }, master_buffer_{ config }, renderer_{ "robin", config }
	, flame_{ config.transformation_ }, alias_{ generateDistribution(config.transformation_) }
{
}

void Engine::run() {
    unsigned int num_threads{ config_.engine_threads_ };
    if (num_threads == 0) { 
        num_threads = std::max(1u, std::thread::hardware_concurrency() - 1);
    }

    std::cout << std::format("Engine running on {} threads\n", num_threads);

    for (unsigned int i{ 0 }; i < num_threads; ++i) {
        workers_.push_back(std::make_unique<Worker>(flame_, alias_, config_, master_buffer_, total_points_));
        workers_.back()->start();
    }

    auto last_telemetry_time = std::chrono::steady_clock::now();
    auto last_frame_time = std::chrono::steady_clock::now();
    uint64_t last_telemetry_points{ 0 };

    while (true) {
        FrameEvents events{ renderer_.fetchUserInput() };
        if (events.quit_) { 
            break; 
        }

        if (events.save_) {
            std::shared_lock lock(master_mutex_);

            auto pixels{ generateTonemap(master_buffer_,
                config_.gui_width_,
                config_.gui_height_) };

            lock.unlock(); // release before the file write
            if (!utils::saveImage(pixels, config_)) {
                std::cerr << "Failed to save image output\n";
            }
            else {
                std::cout << std::format("Saved snapshot to: {}\n", config_.output_name_);
            }
        }

        auto now = std::chrono::steady_clock::now();

        if (now - last_frame_time >= std::chrono::milliseconds(16)) {
            float elapsed{ std::chrono::duration<float>(now - last_telemetry_time).count() };

            if (elapsed >= 0.25f) {
                uint64_t current_points{ total_points_.load(std::memory_order_relaxed) };
                double pps{ (current_points - last_telemetry_points) / elapsed };
                renderer_.updateTelemetry(current_points, pps);
                last_telemetry_time = now;
                last_telemetry_points = current_points;
            }

            {
                std::shared_lock lock(master_mutex_);
                renderer_.updateGUI(master_buffer_);
            }

            last_frame_time = now;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    workers_.clear(); // jthread stop+join on destruction
}
