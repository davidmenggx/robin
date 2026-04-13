#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "robin/color/color.h"
#include "robin/config.h"
#include "robin/constants.h"
#include "robin/utils/save_image.h"

#include <string>
#include <vector>

#include "stb/stb_image_write.h"

bool utils::saveImage(const std::vector<Color>& buffer, Config& config) {
	// width x # of color channels
	int stride{ config.gui_width_ * constants::kColorChannels };

	std::string output_name = config.output_name_ + ".png";

	int success = stbi_write_png(
		output_name.c_str(),
		config.gui_width_,
		config.gui_height_,
		constants::kColorChannels,
		buffer.data(),
		stride
	);

	return success;
}
