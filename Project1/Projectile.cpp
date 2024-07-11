#include "Projectile.h"
#include "Enemy.h"
#include "Player.h"
#include "Obstacle.h"

namespace Game {
	Projectile::Projectile(
		RendererPort* rendererPort_,
		PhysicsEngine* physicsEngine_,
		float positionXInMeters,
		float positionYInMeters
	) : VisualElement(
		rendererPort_, RenderDataDTO{ 
			positionXInMeters, 
			positionYInMeters, 
			10, 
			20, 
			"#ff0000" 
		}
	), physicsEngine(physicsEngine_) {}

	void Projectile::updatePosition() {
		this->positionYInMeters -= this->physicsEngine->calcDisplacement(this->currentSpeedPoints);
	}

	void Projectile::update() {
		this->updatePosition();
	}

	void Projectile::onCollision(VisualElement* otherElement) {
		if (dynamic_cast<Projectile*>(otherElement) || dynamic_cast<Player*>(otherElement)) {
			return;
		}

		if (Enemy* enemy = dynamic_cast<Enemy*>(otherElement)) {
			std::cout << "Enemy position: (" << enemy->getPositionXInMeters() << ", " << enemy->getPositionYInMeters() << ")" << std::endl;
		}
		if (Obstacle* obstacle = dynamic_cast<Obstacle*>(otherElement)) {
			std::cout << "Obstacle position: (" << obstacle->getPositionXInMeters() << ", " << obstacle->getPositionYInMeters() << ")" << std::endl;
		}
	

		this->deleted = true;
	}
}