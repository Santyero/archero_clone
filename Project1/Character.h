#pragma once
#ifndef CHARACTER_H
#define CHARACTER_H
#include <iostream>

#include "VisualElement.h"
#include "PhysicsEngine.h"

namespace Game {
    class Character : public VisualElement {
    private:
        int maxHealth = 100;
        int baseAttackPoints = 0;
        int baseDefensePoints = 0;
        int baseSpeedPoints = 0;
        int baseSpeedAtackPoints = 0;

        float life = 100;
        int currentHealth = 0;
        int currentAttackPoints = 0;
        int currentDefensePoints = 0;
        double currentSpeedPoints = 1;
        int currentSpeedAtackPoints = 0;

    protected:
        PhysicsEngine* physicsEngine = nullptr;

    public:
        Character(RendererPort* adapter, TextureManager* textureManager, const std::string& textureId, PhysicsEngine* physicsEngine_, const RenderDataDTO& renderDataDTOParam);


        float velocity_base = 0.1;
        virtual void attack() = 0;

        void goDown();
        void goUp();
        void goRight();
        void goLeft();
        void stop();


        virtual void onTakeDamage() = 0;
        void setMaxHealth(int maxHealth);
        void setBaseAttackPoints(int baseAttackPoints);
        void setBaseDefensePoints(int baseDefensePoints);
        void setBaseSpeedPoints(int baseSpeedPoints);
        void setBaseSpeedAtackPoints(int baseSpeedAtackPoints);
        void setLife(float life);
        void setAttackPoints(int attackPoints);

        void takeDamage(float damage);

        float getLife();
        AnimationState getState();
        void renderElement() override;

    };
}

#endif // CHARACTER_H
