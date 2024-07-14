#include "Obstacle.h"

namespace Game {
	Obstacle::Obstacle(
		RendererPort* rendererPort_, Vector position, Vector size, Vector velocity
	) : VisualElement(
		rendererPort_, RenderDataDTO{ position, size, velocity, "#ff0000" }
	) {}

	void Obstacle::onCollision(VisualElement* otherElement) {}
}
