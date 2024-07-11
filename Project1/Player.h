#pragma once
#include "Character.h"
#include "Projectile.h"
#include "Obstacle.h"
#include "Skill.h"
#include <vector>
#include <iostream>

namespace Game {
    class Player : public Character {
    public:
        Player(RendererPort* adapter, PhysicsEngine* physicsEngine_, float positionX, float positionY);

        std::vector<Skill*> activeSkills;
        float life = 100;
        bool isInvincible = false;
        double temporaryInvincibilityTime = 0;
        double invincibilityTime = 1000;

        void attack() override;

        void verifyKeyboardCommands();
        void onCollision(VisualElement* otherElement) override;
        void update() override;
    };
}

