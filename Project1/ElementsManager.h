#pragma once
#include <vector>
#include "Enemy.h"
#include "Projectile.h"
#include "Player.h"

namespace Game {
	class ElementsManager {
	private:
		std::vector<Enemy> enemies;
		std::vector<Projectile> projectiles;
	public:

		void renderElements();
	};
}