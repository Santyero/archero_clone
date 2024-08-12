#pragma once
#include <iostream>
#include <string_view>
#include "math-vector.h"
#include <SDL_image.h>

namespace Game {
	using Vector = Mylib::Math::Vector<float, 2>;
	struct RenderDataDTO {
		Vector position;
		Vector size;
		Vector velocity;
		std::string_view hexColor;
		SDL_Surface* image;
		SDL_Rect srcRect;
	};
}