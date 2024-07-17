#include <SDL.h>
#include "Character.h"

using namespace Game;

Character::Character(
    RendererPort* rendererPort_, PhysicsEngine* physicsEngine_, const RenderDataDTO& renderDataDTOParam
) : VisualElement(rendererPort_, renderDataDTOParam), physicsEngine(physicsEngine_) {}

void Character::goDown() {
    this->position.y = this->position.y + this->physicsEngine->calcDisplacement(
        this->velocity.x
    );
}

void Character::goUp() {
    this->position.y = this->position.y - this->physicsEngine->calcDisplacement(
        this->velocity.x
    );
}

void Character::goRight() {
    this->position.x = this->position.x + this->physicsEngine->calcDisplacement(
        this->velocity.y
    );
}

void Character::goLeft() {
    this->position.x = this->position.x - this->physicsEngine->calcDisplacement(
         this->velocity.y
    );

}

void Character::setBaseAttackPoints(int baseAttackPoints) {
	this->baseAttackPoints = baseAttackPoints;
}

void Character::setBaseDefensePoints(int baseDefensePoints) {
	this->baseDefensePoints = baseDefensePoints;
}

void Character::setBaseSpeedPoints(int baseSpeedPoints) {
	this->baseSpeedPoints = baseSpeedPoints;
}

void Character::setBaseSpeedAtackPoints(int baseSpeedAtackPoints) {
	this->baseSpeedAtackPoints = baseSpeedAtackPoints;
}

void Character::setMaxHealth(int maxHealth) {
	this->maxHealth = maxHealth;
	this->currentHealth = maxHealth;
}

void Character::setLife(float life) {
	this->life = life;
}

void Character::setAttackPoints(int attackPoints) {
	this->currentAttackPoints = attackPoints;
}

void Character::takeDamage(float damage) {
	this->life -= damage;
    std::cout << "Life: " << this->life << std::endl;
    this->onTakeDamage();
}

float Character::getLife() {
	return this->life;
}