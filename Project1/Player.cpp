#include <SDL.h>
#include "Player.h"
#include "Enemy.h"

using namespace Game;

Player::Player(RendererPort* adapter, TextureManager* textureManager,
    const std::string& textureId, PhysicsEngine* physicsEngine_,
    Vector position, Vector size) : Character(
		adapter, textureManager, textureId, physicsEngine_,
        RenderDataDTO{ position, size, {0, 0}, "" }
    ) {
    this->setMaxHealth(100);
    this->setLife(100);
	this->velocity_base = 0.2;
}

void Player::attack() {}

void Player::verifyKeyboardCommands() {
    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
    if (not currentKeyStates[SDL_SCANCODE_UP] and not currentKeyStates[SDL_SCANCODE_RIGHT] and not currentKeyStates[SDL_SCANCODE_DOWN] and not currentKeyStates[SDL_SCANCODE_LEFT] and not currentKeyStates[SDL_SCANCODE_RIGHT]) {
        this->stop();
    }
    else {
		this->setAnimationState(AnimationState::WALK);
    }
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

void Player::onCollision(VisualElement* otherElement) {
    if (Enemy* enemy = dynamic_cast<Enemy*>(otherElement)) {
        if (this->isInvincible) {
            return;
        }
        this->takeDamage(10);
        return;
    }
    if (Projectile* projectile = dynamic_cast<Projectile*>(otherElement)) {
        if (this->isInvincible) {
            return;
        }
        this->takeDamage(projectile->damage);
        return;
    }
    if (Obstacle* obstacle = dynamic_cast<Obstacle*>(otherElement)) {
        return;
    }
}

void Player::onTakeDamage() {
    if (this->getLife() <= 0) {
        this->setHexColor("000000");
        std::cout << "Player died" << std::endl;
        return;
    }
    this->isInvincible = true;
    this->temporaryInvincibilityTime = SDL_GetTicks();
    this->setHexColor("ff0000");
}

void Player::update() {
    if (this->isInvincible && SDL_GetTicks() - this->temporaryInvincibilityTime > this->invincibilityTime) {
        if (this->getLife() > 0) {
            this->setHexColor("ffff00");
        }
        this->isInvincible = false;
    }
    VisualElement::update();
}
