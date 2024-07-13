#include <SDL.h>
#include "Character.h"

using namespace Game;

Character::Character(
    RendererPort* rendererPort_, PhysicsEngine* physicsEngine_, const RenderDataDTO& renderDataDTOParam
) : VisualElement(rendererPort_, renderDataDTOParam), physicsEngine(physicsEngine_) {}

void Character::goDown() {
    this->position.y = this->position.y + this->physicsEngine->calcDisplacement(
        this->currentSpeedPoints
    );
}

void Character::goUp() {
    this->position.y = this->position.y - this->physicsEngine->calcDisplacement(
        this->currentSpeedPoints
    );
}

void Character::goRight() {
    this->position.x = this->position.x + this->physicsEngine->calcDisplacement(
        this->currentSpeedPoints
    );
}

void Character::goLeft() {
    this->position.x = this->position.x - this->physicsEngine->calcDisplacement(
        this->currentSpeedPoints
    );

}