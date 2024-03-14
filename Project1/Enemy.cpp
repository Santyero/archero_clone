#include <SDL.h>
#include "Enemy.h"
#include "Character.h"
#include "VisualElementProps.h"
#include <cstdlib>
#include <ctime>
#include "config.h"

using namespace Game;

Enemy::Enemy(RendererPort* adapter) : Character(adapter, RenderDataDTO{ 0, 0, 50, 50, "#ff0000" })
{
    this->randomizePosition();
}

void Enemy::attack()
{
    std::cout << "Player attack" << std::endl;
}

void Enemy::randomizePosition()
{
    float randomX = Config::scenePositionX + (rand() % (Config::sceneWidth - 50));
    float randomY = Config::scenePositionY + (rand() % (Config::sceneHeight - 50));
    std::cout << "randomY" << randomY << std::endl;
    std::cout << "randomX" << randomX << std::endl;
    this->positionXInMeters = randomX;
    this->positionYInMeters = randomY;
}

void Enemy::updateEnemy() {}
