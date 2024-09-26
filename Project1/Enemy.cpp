#include <SDL.h>
#include "Enemy.h"
#include "Character.h"
#include "VisualElementProps.h"
#include <cstdlib>
#include <ctime>
#include "config.h"


namespace Game
{
    Enemy::Enemy(RendererPort* rendererPort_, TextureManager* textureManager, const std::string& textureId, PhysicsEngine* physicsEngine_) : Character(
        rendererPort_, textureManager, textureId, physicsEngine_, RenderDataDTO{ this->randomizePosition(), {50,50}, {0,0}, "#ff9933"}
    ) {
        //this->randomizePosition();
        this->moveDuration = 1000 + rand() % 2000;
        this->shootDuration = 500 + rand() % 1000;
        this->setMaxHealth(100);
        this->setLife(100);
    }

    void Enemy::attack()
    {
        //std::cout << "Player attack" << std::endl;
    }

    Vector Enemy::randomizePosition()
    {
        float sceneWidth = Config::sceneSize.x; // Suponha que essas variáveis existem em Config
        float sceneHeight = Config::sceneSize.y;

        // Define margens para evitar que a posição seja nas bordas vermelhas
        float marginX = 50.0f; // Ajuste conforme necessário para a largura da borda
        float marginY = 50.0f; // Ajuste conforme necessário para a altura da borda
        float marginBottom = sceneHeight / 2; // Ajuste conforme necessário para a margem inferior


        // Calcule os limites máximos para a posição aleatória dentro da área azul
        float minX = marginX;
        float maxX = sceneWidth - marginX;
        float minY = marginY;
        float maxY = sceneHeight - marginBottom;

        // Gere valores aleatórios dentro do intervalo definido
        float randomX = minX + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maxX - minX)));
        float randomY = minY + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maxY - minY)));

        std::cout << "randomX: " << randomX << std::endl;
        std::cout << "randomY: " << randomY << std::endl;

        this->position = { randomX, randomY };

        this->changeDirection();
        return { randomX, randomY };
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
        //std::cout << "Enemy collision" << std::endl;
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
        if (this->getState() == AnimationState::WALK or this->getState() == AnimationState::IDLE) {
            if (currentTime - moveStartTime > moveDuration) {
                std::cout << "Walk" << std::endl;
                this->setAnimationState(AnimationState::SHOOT);
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
        else if (
            this->getState() == AnimationState::SHOOT ||
            this->getState() == AnimationState::SHOOT_ANGULAR_BOTTOM ||
			this->getState() == AnimationState::SHOOT_ANGULAR_TOP ||
			this->getState() == AnimationState::SHOOT_BOTTOM ||
			this->getState() == AnimationState::SHOOT_TOP
        ) {
            this->stop();
            if (currentTime - shootStartTime > shootDuration) {
                std::cout << "Enemy shoot" << std::endl;
                this->setAnimationState(AnimationState::WALK);
			    moveStartTime = currentTime;
			    return;
		    }
		}
        VisualElement::update();
    }

    void Enemy::onTakeDamage() {
        std::cout << "Enemy take damage" << this->getLife() << std::endl;
        if (this->getLife() <= 0) {
            this->hexColor = "#000000";
            this->destroy();
        }
	}
}