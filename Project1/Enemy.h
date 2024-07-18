#pragma once
#ifndef ENEMY_H
#define ENEMY_H

#include "Character.h"
#include "Projectile.h"
#include <iostream>
#include <list>
#include <SDL.h>


namespace Game {
    enum State { MOVING, SHOOTING };
    class Enemy : public Character {

    public:
        Enemy(RendererPort* adapter, PhysicsEngine* physicsEngine_);
        double currentSpeedPoints = 0.1;
        int projectileFramesDelay = 100;
        std::list <Projectile> projectiles;
        
        void attack() override;
        void randomizePosition();
        void onCollision(VisualElement* otherElement) override;
        void update() override;
        void renderProjects();
        void changeDirection();

        void onTakeDamage() override;


    private:
        enum Direction { RIGHT, LEFT, UP, DOWN, UP_RIGHT, UP_LEFT, DOWN_RIGHT, DOWN_LEFT } direction;
        State currentState = MOVING;
        Uint32 moveStartTime = 0;
        Uint32 shootStartTime = 0;

        Uint32 moveDuration;
        Uint32 shootDuration;
    };

}

#endif // ENEMY_H
