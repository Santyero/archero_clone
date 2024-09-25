#include <SDL.h>
#include "Character.h"

namespace Game
{
	Character::Character(
		RendererPort* rendererPort_, TextureManager* textureManager, const std::string& textureId, PhysicsEngine* physicsEngine_, const RenderDataDTO& renderDataDTOParam
	) : VisualElement(rendererPort_, textureManager, textureId, renderDataDTOParam), physicsEngine(physicsEngine_) {}

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

	void Character::setMaxHealth(int max) {
		maxHealth = max;
		if (life > maxHealth) life = maxHealth;
	}

	void Character::setLife(float life) {
		this->life = life;
	}

	void Character::setAttackPoints(int attackPoints) {
		this->currentAttackPoints = attackPoints;
	}

	void Character::takeDamage(float damage) {
		life = std::max(0.0f, life - damage);
		std::cout << "Life: " << this->life << std::endl;
		this->onTakeDamage();
	}

	float Character::getLife() {
		return this->life;
	}

	AnimationState Character::getState() {
		return this->currentState;
	}

	void Character::renderElement() {
		VisualElement::renderElement();

		float healthPercentage = life / maxHealth;
		//std::cout << "Health percentage: " << healthPercentage << std::endl;

		Vector healthBarPosition = {
			position.x,
			position.y + size.y + 5
		};
		Vector healthBarSize = {
			size.x,
			5
		};

		// Barra de fundo (vermelha)
		RenderDataDTO backgroundBar = {
			healthBarPosition,
			healthBarSize,
			{0, 0},
			"#FF0000",  // Vermelho
			nullptr,
			false
		};
		rendererPort->renderElement(backgroundBar);

		// Barra de vida atual (verde)
		Vector currentHealthSize = {
			healthBarSize.x * healthPercentage,
			healthBarSize.y
		};
		RenderDataDTO healthBar = {
			healthBarPosition,
			currentHealthSize,
			{0, 0},
			"#00FF00",  // Verde
			nullptr,
			false
		};
		rendererPort->renderElement(healthBar);
	}
}