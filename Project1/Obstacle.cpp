#include "Obstacle.h"

namespace Game {
	Obstacle::Obstacle(
		RendererPort* rendererPort_, Vector position, Vector size
	) : VisualElement(
		rendererPort_, RenderDataDTO{ position, size, {0,0}, "#ff0000"}
	) {}

	void Obstacle::checkCollision(VisualElement* otherElement) {
		Vector otherElementPosition = otherElement->getPosition();
		Vector otherElementSize = otherElement->getSize();

		if (this->position.x < otherElementPosition.x + otherElementSize.x &&
			this->position.x + this->size.x > otherElementPosition.x &&
			this->position.y < otherElementPosition.y + otherElementSize.y &&
			this->size.y + this->position.y > otherElementPosition.y) {

			this->onCollision(otherElement);
		}
	}

	void Obstacle::onCollision(VisualElement* otherElement) {}
}
