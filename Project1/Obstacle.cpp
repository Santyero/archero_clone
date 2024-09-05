#include "Obstacle.h"

namespace Game {
	Obstacle::Obstacle(
		RendererPort* rendererPort_, TextureManager* textureManager, const std::string& textureId, Vector position, Vector size
	) : VisualElement(
		rendererPort_, textureManager, textureId, RenderDataDTO{ position, size, {0,0}, "#ff0000"}
	) {}

	void Obstacle::onCollision(VisualElement* otherElement) {}
}
