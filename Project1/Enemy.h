#pragma once
#ifndef ENEMY_H
#define ENEMY_H

#include "Character.h"
#include "Projectile.h"
#include <iostream>
#include <list>


namespace Game {
    class Enemy : public Character {

    public:
        Enemy(RendererPort* adapter, PhysicsEngine* physicsEngine_);
        float life = 100;
        double currentSpeedPoints = 0.3;
        int projectileFramesDelay = 300;
        std::list <Projectile> projectiles;
        
        void attack() override;
        void randomizePosition();
        void onCollision(VisualElement* otherElement) override;
        void checkCollision(VisualElement* otherElement) override;
        void update() override;
        void renderProjects();


    private:
        enum Direction { RIGHT, LEFT, UP, DOWN } direction;
    };

}

#endif // ENEMY_H
