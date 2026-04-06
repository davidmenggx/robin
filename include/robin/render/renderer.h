#pragma once

#include "robin/color/accumulation.h"
#include "robin/config.h"
#include "robin/render/frame_events.h"
#include "robin/render/telemetry_hud.h"

#include <string>

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <SDL3_ttf/SDL_ttf.h>

namespace ff {
	class Renderer {
	public:
		Renderer(const std::string& title, const Config& config);
		~Renderer();

		FrameEvents pollEvents();

		void update(Accumulation& buffer);
		void updateTelemetry(int total_points, int current_points_per_second);

	private:
		void displayTelemetry(int total_points, int current_points_per_second);

		const Config& config_;

		SDL_Window* window_{};
		SDL_Renderer* renderer_{};
		SDL_Texture* texture_{};

		SDL_Window* telemetry_window_{};
		SDL_Renderer* telemetry_renderer_{};

		TelemetryHUD hud_{};

		enum class TextAlignment { kLeft, kMiddle, kRight };
		TTF_Font* font_{};

		void drawText(const std::string& text, float x, float y, TextAlignment align);
	};
}
