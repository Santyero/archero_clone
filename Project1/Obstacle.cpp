#include "Obstacle.h"

namespace Game {
	Obstacle::Obstacle(
		RendererPort* rendererPort_, float positionXInMeters, float positionYInMeters
	) : VisualElement(
		rendererPort_, RenderDataDTO{ positionXInMeters, positionYInMeters, 50, 50, "#ff0000" }
	) {}

	void Obstacle::onCollision(VisualElement* otherElement) {}
}