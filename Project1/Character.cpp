#include <SDL.h>
#include "Character.h"

namespace Game
{
	Character::Character(
		RendererPort* rendererPort_, PhysicsEngine* physicsEngine_, const RenderDataDTO& renderDataDTOParam
	) : VisualElement(rendererPort_, renderDataDTOParam), physicsEngine(physicsEngine_) {}

	void Character::goDown() {
		this->velocity.y = 0.1;
	}

	void Character::goUp() {
		this->velocity.y = -0.1;
	}

	void Character::goRight() {
		this->velocity.x = 0.1;
	}

	void Character::goLeft() {
		this->velocity.x = -0.1;

	}

	void Character::stop() {
		this->velocity = { 0, 0 };
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

	State Character::getState() {
		return this->currentState;
	}
}