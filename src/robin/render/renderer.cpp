#include "color/accumulation.h"
#include "color/color.h"
#include "constants.h"
#include "render/renderer.h"
#include "render/tonemap.h"

#include <format>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_video.h>
#include <SDL3_ttf/SDL_ttf.h>

using namespace ff;

Renderer::Renderer(const std::string& title, bool show_telemetry) 
	: show_telemetry_{ show_telemetry }
{
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		throw std::runtime_error(std::string("SDL initialization error: ") + SDL_GetError());
	}

	if (!SDL_CreateWindowAndRenderer(
		title.c_str(), constants::kWidth, constants::kHeight,
		0, &window_, &renderer_)) {
		SDL_Quit();
		throw std::runtime_error(std::string("Window/Renderer creation failed: ") + SDL_GetError());
	}

	texture_ = SDL_CreateTexture(renderer_, SDL_PIXELFORMAT_RGBA32,
		SDL_TEXTUREACCESS_STREAMING, constants::kWidth, constants::kHeight);

	if (!texture_) {
		SDL_DestroyRenderer(renderer_);
		SDL_DestroyWindow(window_);
		SDL_Quit();
		throw std::runtime_error(std::string("Texture creation failed: ") + SDL_GetError());
	}

	if (show_telemetry_) {
		telemetry_window_ = SDL_CreateWindow("robin runtime telemetry", 
			constants::kTelemetryWidth, constants::kTelemetryHeight, 0);
		telemetry_renderer_ = SDL_CreateRenderer(telemetry_window_, nullptr);
	}

	TTF_Init();
	std::string font_path{ std::string(PROJECT_ROOT_DIR) + "/assets/fonts/RobotoMono-VariableFont_wght.ttf" };
	font_ = TTF_OpenFont(font_path.c_str(), 14);
}

Renderer::~Renderer() {
	if (texture_) {
		SDL_DestroyTexture(texture_);
	}
	if (renderer_) {
		SDL_DestroyRenderer(renderer_);
	}
	if (window_) {
		SDL_DestroyWindow(window_);
	}
	SDL_Quit();
}

// returns false if the user closes the application
bool Renderer::PollEvents() {
	SDL_Event event{};
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_EVENT_QUIT) {
			return false;
		}
		if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
			return false;
		}
		if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE) {
			return false;
		}
	}
	return true;
}

void Renderer::Update(Accumulation& buffer) {
	std::vector<Color> colors{ GenerateTonemap(buffer) };

	SDL_UpdateTexture(texture_, nullptr, colors.data(), constants::kWidth * sizeof(Color));
	SDL_RenderClear(renderer_);
	SDL_RenderTexture(renderer_, texture_, nullptr, nullptr);
	SDL_RenderPresent(renderer_);
}

void Renderer::UpdateTelemetry(int total_points, int current_points_per_second) {
	hud_.Push(current_points_per_second);
	DisplayTelemetry(total_points, current_points_per_second);
}

void Renderer::DrawText(const std::string& text, float x, float y, TextAlignment align) {
	SDL_Color white = { 210, 210, 210, 255 };
	SDL_Surface* surf{ TTF_RenderText_Blended(font_, text.c_str(), 0, white) };
	SDL_Texture* tex{ SDL_CreateTextureFromSurface(telemetry_renderer_, surf) };

	float width{};
	float height{};
	SDL_GetTextureSize(tex, &width, &height);

	float x_offset{ x };
	if (align == TextAlignment::kMiddle) {
		x_offset = x - width / 2.0f;
	}
	else if (align == TextAlignment::kRight) {
		x_offset = x - width;
	}

	SDL_FRect dst{ x_offset, y, width, height };
	SDL_RenderTexture(telemetry_renderer_, tex, nullptr, &dst);

	SDL_DestroyTexture(tex);
	SDL_DestroySurface(surf);
}

void Renderer::DisplayTelemetry(int total_points, int current_points_per_second) {
	if (!show_telemetry_) {
		return;
	}

	SDL_SetRenderDrawColor(telemetry_renderer_, 20, 20, 20, 255);
	SDL_RenderClear(telemetry_renderer_);

	const int plot_x{ constants::kTelemetryMarginLeft };
	const int plot_y{ constants::kTelemetryMarginTop };
	const int plot_w{ constants::kTelemetryWidth -
		constants::kTelemetryMarginLeft - constants::kTelemetryMarginRight };
	const int plot_h{ constants::kTelemetryHeight - 
		constants::kTelemetryMarginTop - constants::kTelemetryMarginBottom };

	SDL_SetRenderDrawColor(telemetry_renderer_, 80, 80, 80, 255);
	SDL_RenderLine(telemetry_renderer_,
		plot_x, plot_y,
		plot_x, plot_y + plot_h);

	SDL_RenderLine(telemetry_renderer_,
		plot_x, plot_y + plot_h,
		plot_x + plot_w, plot_y + plot_h);

	for (int i{ 0 }; i <= constants::kTelemetryTicks; ++i) {
		// render just the axes and labels
		float normalized_vertical_position{ static_cast<float>(i) / constants::kTelemetryTicks };
		int pixel_y{ plot_y + plot_h - static_cast<int>(normalized_vertical_position * plot_h) };
		
		float points_per_second{ static_cast<float>(normalized_vertical_position * 
			hud_.max_points_per_second_ * constants::kTelemetryGraphUpperScaling / 1e6) };

		// tick
		SDL_SetRenderDrawColor(telemetry_renderer_, 80, 80, 80, 255);
		SDL_RenderLine(telemetry_renderer_, plot_x - 4, pixel_y, plot_x, pixel_y);

		// gridline
		SDL_SetRenderDrawColor(telemetry_renderer_, 35, 35, 35, 255);
		SDL_RenderLine(telemetry_renderer_, plot_x, pixel_y, plot_x + plot_w, pixel_y);

		// label
		std::string label{ std::format("{:.1f}M", points_per_second) };
		DrawText(label, plot_x - 6, pixel_y, TextAlignment::kRight);
	}

	SDL_SetRenderDrawColor(telemetry_renderer_, 80, 200, 120, 255);

	// render the data on top of the graph
	std::optional<SDL_FPoint> prev;
	for (int i{ 0 }; i < constants::kTelemetryHistorySize; ++i) {
		int idx{ (hud_.head_ + i) % constants::kTelemetryHistorySize };
		float sample{ hud_.samples_[idx] };
		if (sample == 0.0) {
			prev.reset(); 
			continue; 
		}

		float top_level{ static_cast<float>(sample / (hud_.max_points_per_second_ * constants::kTelemetryGraphUpperScaling)) };
		float px_{ plot_x + static_cast<float>(i) / (constants::kTelemetryHistorySize - 1) * plot_w };
		float py_{ plot_y + plot_h - top_level * plot_h };

		if (prev) {
			SDL_RenderLine(telemetry_renderer_, prev->x, prev->y, px_, py_);
		}
		prev = SDL_FPoint{ px_, py_ };
	}

	// summary stats
	DrawText(
		std::format("points drawn/sec    {:.3f}M", static_cast<float>(current_points_per_second) / 1e6),
		constants::kTelemetryWidth / 2, 14, TextAlignment::kMiddle);
	DrawText(
		std::format("total points drawn    {:.1f}M", static_cast<float>(total_points) / 1e6),
		constants::kTelemetryWidth / 2, 32, TextAlignment::kMiddle);

	SDL_RenderPresent(telemetry_renderer_);
}
