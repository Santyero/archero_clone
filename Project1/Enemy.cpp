#include <SDL.h>
#include "Enemy.h"
#include "Character.h"
#include "VisualElementProps.h"
#include <cstdlib>
#include <ctime>
#include "config.h"

namespace Game
{
    Enemy::Enemy(RendererPort* rendererPort_, PhysicsEngine* physicsEngine_) : Character(
        rendererPort_, physicsEngine_, RenderDataDTO{ {0,0}, {50,50}, {0.1,0.1}, "#ff9933"}
    ) {
        this->randomizePosition();
        this->moveDuration = 1000 + rand() % 2000;
        this->shootDuration = 500 + rand() % 1000;
    }

    void Enemy::attack()
    {
        std::cout << "Player attack" << std::endl;
    }

    void Enemy::randomizePosition()
    {
        float randomX = Config::scenePosition.x + (rand() % int(Config::scenePosition.x - 50));
        float randomY = Config::scenePosition.y + (rand() % int(Config::scenePosition.y - 50));
        std::cout << "randomY" << randomY << std::endl;
        std::cout << "randomX" << randomX << std::endl;
        this->position = { randomX, randomY };

        this->changeDirection();
    }

    void Enemy::changeDirection() {
        int randomDirection = rand() % 8;
        switch (randomDirection) {
        case 0: direction = RIGHT; break;
        case 1: direction = LEFT; break;
        case 2: direction = UP; break;
        case 3: direction = DOWN; break;
        case 4: direction = UP_RIGHT; break;
        case 5: direction = UP_LEFT; break;
        case 6: direction = DOWN_RIGHT; break;
        case 7: direction = DOWN_LEFT; break;
        }
    }

    void Enemy::onCollision(VisualElement* otherElement)
    {
        std::cout << "Enemy collision" << std::endl;
        if (Projectile* projectile = dynamic_cast<Projectile*>(otherElement)) {
            std::cout << "Enemy take damage" << projectile->damage << std::endl;
			this->takeDamage(projectile->damage);
            return;
		}

        Vector otherElementPosition = otherElement->getPosition();
        Vector otherElementSize = otherElement->getSize();

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
        changeDirection();
    }


    void Enemy::update()
    {
        Uint32 currentTime = SDL_GetTicks();
        if (currentState == MOVING) {
            if (currentTime - moveStartTime > moveDuration) {
                currentState = SHOOTING;
                shootStartTime = currentTime;
                return;
            }
            switch (direction) {
            case RIGHT:
                this->goRight();
                break;
            case LEFT:
                this->goLeft();
                break;
            case UP:
                this->goUp();
                break;
            case DOWN:
                this->goDown();
                break;
            case UP_RIGHT:
                this->goUp();
                this->goRight();
                break;
            case UP_LEFT:
                this->goUp();
                this->goLeft();
                break;
            case DOWN_RIGHT:
                this->goDown();
                this->goRight();
                break;
            case DOWN_LEFT:
                this->goDown();
                this->goLeft();
                break;
            }
        }
        else if (currentState == SHOOTING) {
            if (currentTime - shootStartTime > shootDuration) {
				currentState = MOVING;
				moveStartTime = currentTime;
				return;
			}
			//this->renderProjects();
		}

    }

    void Enemy::onTakeDamage() {
        std::cout << "Enemy take damage" << this->getLife() << std::endl;
        if (this->getLife() <= 0) {
            this->hexColor = "#000000";
            this->destroy();
        }
	}

    void Enemy::renderProjects() {
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
                velocity,
                10
            ));
            projectileFramesDelay = 300;
        }
    }
}