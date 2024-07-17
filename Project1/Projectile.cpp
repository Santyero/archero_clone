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

	void Projectile::updatePosition() {
		if (this->position.y > Config::scenePosition.y + Config::sceneSize.y or this->position.y < Config::scenePosition.y - Config::sceneSize.y) {
			this->destroy();
		}
		this->position.y -= this->physicsEngine->calcDisplacement(this->velocity.x);
	}

	void Projectile::update() {
		this->updatePosition();
	}

	void Projectile::destroy() {
		this->deleted = true;
	}

	void Projectile::onCollision(VisualElement* otherElement) {
		if (dynamic_cast<Projectile*>(otherElement) || dynamic_cast<Player*>(otherElement)) {
			return;
		}

		if (Enemy* enemy = dynamic_cast<Enemy*>(otherElement)) {
			//std::cout << "Enemy position: (" << enemy->getPosition().x << ", " << enemy->getPosition().y << ")" << std::endl;
		}
		if (Obstacle* obstacle = dynamic_cast<Obstacle*>(otherElement)) {
			//std::cout << "Obstacle position: (" << obstacle->getPosition().x << ", " << obstacle->getPosition().y << ")" << std::endl;
		}
	

		this->deleted = true;
	}

	void Projectile::checkCollision(VisualElement* otherElement) {
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