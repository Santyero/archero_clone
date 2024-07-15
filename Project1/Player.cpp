#include <SDL.h>
#include "Player.h"
#include "Enemy.h"

using namespace Game;


Player::Player(
    RendererPort* rendererPort_, PhysicsEngine* physicsEngine_, Vector position, Vector size
) : Character(rendererPort_, physicsEngine_, RenderDataDTO{ position, size, {1,1}, "#00ff00" }) {}

void Player::attack() {
}

void Player::verifyKeyboardCommands() {
    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
    if (currentKeyStates[SDL_SCANCODE_UP]) {
        this->goUp();
    }
    if (currentKeyStates[SDL_SCANCODE_DOWN]) {
        this->goDown();
    }
    if (currentKeyStates[SDL_SCANCODE_LEFT]) {
        this->goLeft();
    }
    if (currentKeyStates[SDL_SCANCODE_RIGHT]) {
        this->goRight();
    }
}

void Player::checkCollision(VisualElement* otherElement) {

    Vector otherElementPosition = otherElement->getPosition();
    Vector otherElementSize = otherElement->getSize();

    if (this->position.x < otherElementPosition.x + otherElementSize.x &&
        this->position.x + this->size.x > otherElementPosition.x &&
        this->position.y < otherElementPosition.y + otherElementSize.y &&
        this->size.y + this->position.y > otherElementPosition.y) {

        float overlapLeft = (this->position.x + this->size.y) - otherElementPosition.x;
        float overlapRight = (otherElementPosition.x + otherElementSize.y) - this->position.x;
        float overlapTop = (this->position.y + this->size.x) - otherElementPosition.y;
        float overlapBottom = (otherElementPosition.y + otherElementSize.x) - this->position.y;

        float minOverlapX = std::min(overlapLeft, overlapRight);
        float minOverlapY = std::min(overlapTop, overlapBottom);

        if (minOverlapX < minOverlapY) {
            if (overlapLeft < overlapRight) {
                this->position.x -= overlapLeft;
            }
            else {
                this->position.x += overlapRight;
            }
        }
        else {
            if (overlapTop < overlapBottom) {
                this->position.y -= overlapTop;
            }
            else {
                this->position.y += overlapBottom;
            }
        }

        this->onCollision(otherElement);
    }
}

void Player::onCollision(VisualElement* otherElement) {
    if (Projectile* projectile = dynamic_cast<Projectile*>(otherElement)) {
        return;
    }

    if (Enemy* enemy = dynamic_cast<Enemy*>(otherElement)) {
        this->takeDamage(10);
        return;
	}

    if (Obstacle* obstacle = dynamic_cast<Obstacle*>(otherElement)) {
        return;
	}

}

void Player::takeDamage(float damage) {
    if (this->isInvincible) {
		return;
	}
	this->life -= damage;
    if (this->life <= 0) {
		// this->deleted = true;
        this->hexColor = "00000";
        std::cout << "Player died" << std::endl;
	}
    else {
        this->isInvincible = true;
        this->temporaryInvincibilityTime = SDL_GetTicks();
        this->hexColor = "ff0000";
    }
}   

void Player::update() {
    //std::cout << "Player update" << std::endl;
    if (this->isInvincible && SDL_GetTicks() - this->temporaryInvincibilityTime > this->invincibilityTime) {
        if (this->life > 0) {
			this->hexColor = "ffff00";
		}
		this->isInvincible = false;
	}
}

void Player::renderProjects() {
    if (this->projectileFramesDelay > 0) {
        this->projectileFramesDelay--;
    }
    else {
        Vector projectilePosition = { this->position.x + 25, this->position.y - 20 };
        Vector size = { 10, 10 };
        Vector velocity = { 0, -0.1 };
        this->projectiles.emplace_back(Projectile(
            this->rendererPort,
            this->physicsEngine,
            projectilePosition,
            size,
            velocity
        ));
        projectileFramesDelay = 300;
    }
}

