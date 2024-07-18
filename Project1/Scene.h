#pragma once
#include "VisualElement.h"

namespace Game {
	class Scene : public VisualElement
	{
	public:
		Scene(RendererPort* adapter);
		void onCollision(VisualElement* otherElement) override;
		void update() override {}

	};
}