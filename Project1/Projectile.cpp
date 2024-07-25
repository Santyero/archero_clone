#include "Projectile.h"
#include "Enemy.h"
#include "Player.h"
#include "Obstacle.h"
#include "config.h"

namespace Game {
	Projectile::Projectile(
		RendererPort* rendererPort_,
		PhysicsEngine* physicsEngine_,
		Vector position,
		Vector size,
		Vector velocity,
		float damage
	) : VisualElement(
		rendererPort_, RenderDataDTO{
			position,
			size,
			velocity,
			"#ff0000"
		}
	), physicsEngine(physicsEngine_) {
		this->damage = damage;
	}

	void Projectile::update() {}

	void Projectile::onCollision(VisualElement* otherElement) {
		/*if (dynamic_cast<Projectile*>(otherElement) || dynamic_cast<Player*>(otherElement)) {
			return;
		}*/
		this->destroy();
	}

}