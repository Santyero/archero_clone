#pragma once
#ifndef ENEMY_H
#define ENEMY_H

#include "Character.h"
#include <iostream>

namespace Game {
    class Enemy : public Character {

    public:
        Enemy(RendererPort* adapter, PhysicsEngine* physicsEngine_);
        float life = 100;
        void attack() override;
        void randomizePosition();
        void updateEnemy();
        void onCollision() override;
        void update() override;

    private:
        enum Direction { RIGHT, LEFT, UP, DOWN } direction;
    };

}

#endif // ENEMY_H
