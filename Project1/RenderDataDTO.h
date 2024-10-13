#pragma once
#include <iostream>
#include "math-vector.h"
#include <SDL_image.h>

namespace Game {
	using Vector = Mylib::Math::Vector<float, 2>;
	struct RenderDataDTO {
		Vector position;
		Vector size;
		Vector velocity;
		std::string hexColor;
		SDL_Texture* texture;
		bool flipHorizontally;
	};
}