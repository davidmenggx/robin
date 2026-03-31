#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

constexpr int kWidth{1000};
constexpr int kHeight{1000};

int main(int argc, char** argv) { 
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("SDL initialization error: %s", SDL_GetError());
    return 1;
  }

  SDL_Window* window{nullptr};
  SDL_Renderer* renderer{nullptr};

  if (!SDL_CreateWindowAndRenderer("robin", kWidth, kWidth, 0, &window, &renderer)) {
    SDL_Log("Window or renderer error: %s", SDL_GetError());
    SDL_Quit();
    return 1;
  }

  bool running{true};
  SDL_Event event{};

  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT || 
          (event.type == SDL_EVENT_KEY_DOWN && 
              event.key.key == SDLK_ESCAPE)) {
        running = false;
      }
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  window = nullptr;
  renderer = nullptr;

  SDL_Quit();

  return 0;
}
