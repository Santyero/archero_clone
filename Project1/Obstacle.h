#pragma once
#include "VisualElement.h"

namespace Game {
	class Obstacle : public VisualElement {
	public:
		Obstacle(
			RendererPort* rendererPort_, float positionXInMeters, float positionYInMeters
		);

		void onCollision(VisualElement* otherElement) override;

		void update() override {}
	};
}