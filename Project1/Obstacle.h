#pragma once
#include "VisualElement.h"

namespace Game {
	class Obstacle : public VisualElement {
	public:
		Obstacle(
			RendererPort* rendererPort_, Vector position, Vector size, Vector velocity
		);

		void onCollision(VisualElement* otherElement) override;

		void update() override {}
	};
}