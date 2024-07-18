#include <SDL.h>
#include "Player.h"
#include "Enemy.h"

using namespace Game;


Player::Player(
    RendererPort* rendererPort_, PhysicsEngine* physicsEngine_, Vector position, Vector size
) : Character(rendererPort_, physicsEngine_, RenderDataDTO{ position, size, {1,1}, "#00ff00" }) {
    this->setMaxHealth(100);
    this->setLife(100);
}

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
    if (not currentKeyStates[SDL_SCANCODE_RIGHT] and not currentKeyStates[SDL_SCANCODE_DOWN] and not currentKeyStates[SDL_SCANCODE_LEFT] and not currentKeyStates[SDL_SCANCODE_RIGHT]) {
        this->spawnProjects();
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

void Player::spawnProjects() {
    if (this->projectileFramesDelay > 0) {
        this->projectileFramesDelay--;
    }
    else {
        // fazer assim depois, mas fazer no Game Object
        //VisualElement* inimigo;
       // Vector projectVelocity2 = inimigo->getPosition() - this->position;
       // projectVelocity2.set_length(0.5); // seta a velocidade do projetil


        Vector projectilePosition = { this->position.x + 25, this->position.y - 20 };
        Vector projectileSize = { 10, 10 };
        Vector projectileVelocity = { 0.5, 0.5 };
        this->projectiles.emplace_back(Projectile(
            this->rendererPort,
            this->physicsEngine,
            projectilePosition,
            projectileSize,
            projectileVelocity,
            10
        ));
        projectileFramesDelay = 300;
    }
}

