#include <SDL.h>
#include "Player.h"
#include "Enemy.h"

using namespace Game;
Player::Player(
    RendererPort* rendererPort_, PhysicsEngine* physicsEngine_, Vector position, Vector size, SDL_Surface*  player_img, const AnimationConfig& animConfig
) : Character(rendererPort_, physicsEngine_, RenderDataDTO{ position, size, {0, 0}, "#00ff00", player_img }), animationConfig(animConfig)  {
    if (player_img == nullptr) {
        std::cerr << "Unable to load image! SDL_image Error: " << IMG_GetError() << std::endl;
        throw std::runtime_error("Ocorreu um erro!");
    }
    
    this->setMaxHealth(100);
    this->setLife(100);

  
    this->setFrames(animationConfig.idleFrames);
}

void Player::attack() {
}

void Player::verifyKeyboardCommands() {
    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
    if (not currentKeyStates[SDL_SCANCODE_UP] and not currentKeyStates[SDL_SCANCODE_RIGHT] and not currentKeyStates[SDL_SCANCODE_DOWN] and not currentKeyStates[SDL_SCANCODE_LEFT] and not currentKeyStates[SDL_SCANCODE_RIGHT]) {
        this->stop();
        this->currentState = SHOOTING;
        this->setFrames(animationConfig.idleFrames);
    }
    else {
        this->currentState = MOVING;
        this->setFrames(animationConfig.runningFrames);
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
		this->hexColor = "000000";
        std::cout << "Player died" << std::endl;
        return;
	}
	this->isInvincible = true;
	this->temporaryInvincibilityTime = SDL_GetTicks();
	this->hexColor = "ff0000";
}


void Player::update() {
     if (this->isInvincible && SDL_GetTicks() - this->temporaryInvincibilityTime > this->invincibilityTime) {
        if (this->getLife() > 0) {
            this->hexColor = "ffff00";
        }
        this->isInvincible = false;
    }
}


