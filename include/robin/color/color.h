#pragma once

#include <cstdint>

struct Color {
	uint8_t red_{};
	uint8_t green_{};
	uint8_t blue_{};
	uint8_t a_{ 255 }; // sdl3 expects rgba format. for this project im keeping a at 255
};
