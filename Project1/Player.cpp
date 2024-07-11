#include <SDL.h>
#include "Player.h"


namespace Game {
    Player::Player(
        RendererPort* rendererPort_, PhysicsEngine* physicsEngine_, float positionXInMeters, float positionYInMeters
    ) : Character(rendererPort_, physicsEngine_, RenderDataDTO{ positionXInMeters, positionYInMeters, 50, 50, "#ffff00" }) {}

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

    void Player::onCollision(VisualElement* otherElement) {
    	if (Projectile* projectile = dynamic_cast<Projectile*>(otherElement)) {
            return;
        }
	    //std::cout << "Player collision" << std::endl;
        
        
        if (!this->isInvincible) {
            this->isInvincible = true;
            this->life -= 10;
            this->hexColor = "808080";
            this->temporaryInvincibilityTime = SDL_GetTicks();

            if (this->life <= 0) {
                this->hexColor = "00000";
                std::cout << "Player died" << std::endl;
            }
            else {
                std::cout << "Player life: " << this->life << std::endl;
            }
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
}
