#include "Obstacle.h"

namespace Game {
	Obstacle::Obstacle(
		RendererPort* rendererPort_, Vector position, Vector size
	) : VisualElement(
		rendererPort_, RenderDataDTO{ position, size, {0,0}, "#ff0000"}
	) {}

	void Obstacle::onCollision(VisualElement* otherElement) {}
}
