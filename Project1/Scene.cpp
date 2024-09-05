#include "Scene.h"
#include "config.h"

namespace Game
{
	Scene::Scene(RendererPort* adapter, TextureManager* textureManager, const std::string& textureId): VisualElement(
		adapter,
		textureManager, 
		textureId,
		RenderDataDTO{
		Config::scenePosition,
		Config::sceneSize,
		{0, 0},
		"#9999FF"
	}) {}

	void Scene::onCollision(VisualElement* otherElement)
	{}

}
