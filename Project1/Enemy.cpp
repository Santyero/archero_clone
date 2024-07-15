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
        rendererPort_, physicsEngine_, RenderDataDTO{ {0,0}, {50,50}, {1,1}, "#ff9933"}
    ) {
        this->randomizePosition();
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

        int randomDirection = rand() % 4;
        switch (randomDirection) {
        case 0: direction = RIGHT; break;
        case 1: direction = LEFT; break;
        case 2: direction = UP; break;
        case 3: direction = DOWN; break;
        }
    }

    void Enemy::onCollision(VisualElement* otherElement)
    {
        std::cout << "Enemy collision" << std::endl;
        // Mudar a dire��o aleatoriamente em caso de colis�o
        int randomDirection = rand() % 4;
        switch (randomDirection) {
        case 0: direction = RIGHT; break;
        case 1: direction = LEFT; break;
        case 2: direction = UP; break;
        case 3: direction = DOWN; break;
        }
    }

    void Enemy::checkCollision(VisualElement* otherElement) {


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

    void Enemy::update()
    {
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
        }
    }
}