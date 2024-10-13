#pragma once
#include "Character.h"
#include "Projectile.h"
#include "Obstacle.h"
#include <vector>
#include <iostream>
#include <list>
#include "AnimationConfig.h"
#include "TextureManager.h"

namespace Game {
    class Player : public Character {
    public:
        Player() = default;
        Player(RendererPort* adapter, TextureManager* textureManager,
            const std::string& textureId, PhysicsEngine* physicsEngine_,
            Vector position, Vector size);

        bool isInvincible = false;
        double temporaryInvincibilityTime = 0;
        double invincibilityTime = 1000;

        void attack() override;
        void verifyKeyboardCommands();
        void onCollision(VisualElement* otherElement) override;
        void update() override;
        void onTakeDamage() override;

    private:
        TextureManager* textureManager;
        std::string textureId;
        int currentFrame = 0;
        AnimationInfo animationInfo;

        //std::string getCurrentAnimationState() const;
        //int getCurrentFrameIndex() const;
    };
}