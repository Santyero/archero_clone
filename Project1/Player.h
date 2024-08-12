#pragma once
#include "Character.h"
#include "Projectile.h"
#include "Obstacle.h"
#include "Skill.h"
#include <vector>
#include <iostream>
#include <list>
#include "AnimationConfig.h"

namespace Game {
    class Player : public Character {
    public:
        Player() = default;
        Player(RendererPort* adapter, PhysicsEngine* physicsEngine_, Vector position, Vector size, SDL_Surface* player_img, const AnimationConfig& animConfig);

        std::vector<Skill*> activeSkills;
        bool isInvincible = false;
        double temporaryInvincibilityTime = 0;
        double invincibilityTime = 1000;

        void attack() override;

        void verifyKeyboardCommands();
        void onCollision(VisualElement* otherElement) override;
        void update() override;
        void onTakeDamage() override;
    private:
        AnimationConfig animationConfig;
    };
}

