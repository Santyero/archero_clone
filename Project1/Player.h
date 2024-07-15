#pragma once
#include "Character.h"
#include "Projectile.h"
#include "Obstacle.h"
#include "Skill.h"
#include <vector>
#include <iostream>
#include <list>

namespace Game {
    class Player : public Character {
    public:
        Player() = default;
        Player(RendererPort* adapter, PhysicsEngine* physicsEngine_, Vector position, Vector size);

        std::vector<Skill*> activeSkills;
        float life = 100;
        bool isInvincible = false;
        double temporaryInvincibilityTime = 0;
        double invincibilityTime = 1000;

        int projectileFramesDelay = 300;
        std::list <Projectile> projectiles;

        void attack() override;

        void verifyKeyboardCommands();
        void onCollision(VisualElement* otherElement) override;
        void checkCollision(VisualElement* otherElement) override;
        void update() override;
        void takeDamage(float damage);
        void renderProjects();
    };
}

