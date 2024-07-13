#pragma once
#include "VisualElement.h"
#include "RendererPort.h"
#include "RenderDataDTO.h"
#include "PhysicsEngine.h"

namespace Game {
	class Projectile: public VisualElement {
	private:
		float currentSpeedPoints = 0.1;
		PhysicsEngine* physicsEngine = nullptr;
		
		void updatePosition();
	public:
		Projectile(
			RendererPort* rendererPort_,
			PhysicsEngine* physicsEngine_,
			Vector position,
			Vector size,
			Vector velocity
		);


		void onCollision(VisualElement* otherElement) override;

		void update() override;
	};
}