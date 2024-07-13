#include <SDL.h>
#include "Enemy.h"
#include "Character.h"
#include "VisualElementProps.h"
#include <cstdlib>
#include <ctime>
#include "config.h"

namespace Game
{
    Vector velocity = { 0, 0 };
    Vector position = { 0, 0 };
    Vector size = { 50, 50 };

    Enemy::Enemy(RendererPort* rendererPort_, PhysicsEngine* physicsEngine_) : Character(
        rendererPort_, physicsEngine_, RenderDataDTO{ position, size, velocity, "#ff9933" }
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

    void Enemy::updateEnemy() {}


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