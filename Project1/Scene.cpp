#include "Scene.h"
#include "config.h"

namespace Game
{
	Scene::Scene(RendererPort* adapter) : VisualElement(adapter, RenderDataDTO{
		Config::scenePosition,
		Config::sceneSize,
		{0, 0},
		"#9999FF"
	}) {}

	void Scene::onCollision(VisualElement* otherElement)
	{
		std::cout << "Scene collision" << std::endl;
	}

	void Scene::checkCollision(VisualElement* otherElement)
	{
		Vector otherElementPosition = otherElement->getPosition();
		Vector otherElementSize = otherElement->getSize();

		if (this->position.x < otherElementPosition.x + otherElementSize.x &&
			this->position.x + this->size.x > otherElementPosition.x &&
			this->position.y < otherElementPosition.y + otherElementSize.y &&
			this->size.y + this->position.y > otherElementPosition.y) {
			this->onCollision(otherElement);
		}
	}
}
