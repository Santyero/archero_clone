#pragma once
#include <iostream>
#include <string_view>
#include "math-vector.h"

namespace Game {
	using Vector = Mylib::Math::Vector<float, 2>;
	struct RenderDataDTO {
		Vector position;
		Vector size;
		Vector velocity;
		std::string_view hexColor;
	};
}