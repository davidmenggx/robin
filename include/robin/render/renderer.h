#pragma once

#include "color/accumulation.h"

#include <string>

#include <SDL3/SDL.h>

namespace ff {
	class Renderer {
	public:
		Renderer(const std::string& title);
		~Renderer();

		// returns false if the user closes the application
		bool PollEvents();

		void Update(Accumulation& buffer);

	private:
		SDL_Window* window_{};
		SDL_Renderer* renderer_{};
		SDL_Texture* texture_{};
	};
}
