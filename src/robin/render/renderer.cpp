#include "color/accumulation.h"
#include "color/color.h"
#include "constants.h"
#include "render/renderer.h"
#include "render/tonemap.h"

#include <stdexcept>
#include <string>
#include <vector>

#include <SDL3/SDL.h>

using namespace ff;

Renderer::Renderer(const std::string& title) {
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
