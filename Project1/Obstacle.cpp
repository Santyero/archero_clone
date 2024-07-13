#include "Obstacle.h"

namespace Game {
	Vector velocity = Vector(0, 0);
	Obstacle::Obstacle(
		RendererPort* rendererPort_, Vector position, Vector size
	) : VisualElement(
		rendererPort_, RenderDataDTO{ position, size, velocity, "#ff0000" }
	) {}

	void Obstacle::onCollision(VisualElement* otherElement) {}
}
