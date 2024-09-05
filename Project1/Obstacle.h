#pragma once
#include "VisualElement.h"

namespace Game {
	class Obstacle : public VisualElement {
	public:
		Obstacle(
			RendererPort* rendererPort_, TextureManager* textureManager, const std::string& textureId, Vector position, Vector size
		);

		void onCollision(VisualElement* otherElement) override;

		void update() override {}
	};
}