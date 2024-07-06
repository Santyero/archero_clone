#pragma once
#include "Character.h"
#include "Skill.h"
#include <vector>
#include <iostream>

namespace Game {
    class Player : public Character {
    public:
        Player(RendererPort* adapter, PhysicsEngine* physicsEngine_, float positionX, float positionY);

        std::vector<Skill*> activeSkills;

        void attack() override;

        void verifyKeyboardCommands();
        void onCollision(VisualElement* otherElement) override;
        void update() override {}
    };
}

