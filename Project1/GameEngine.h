#pragma once
#ifndef GAME_H
#define GAME_H

#include "Window.h"
#include "Enemy.h"
#include "TimeServicePort.h"
#include "RendererPort.h"
#include "PhysicsEngine.h"
#include "Obstacle.h"
#include <list>

class Player;

namespace Game {
	class Player;

	class GameEngine
	{
	private:
		Window& window;
		RendererPort* rendererPort = nullptr;
		TimeServicePort* timeServicePort = nullptr;
		PhysicsEngine* physicsEngine = nullptr;
		std::list <Enemy> enemies;
		std::list <Obstacle> obstacles;
		std::list <Projectile> projectiles;
		Player* player = nullptr;

		
	public:
		GameEngine(Window& window_, RendererPort* rendererPort_, TimeServicePort* timeServicePort_);

		void createWall();
		void createEnemies();

		void startGame();
		void loadElements();
		void updateCollisions();
		VisualElement* findNextElement(VisualElement* selectedElement, std::list<VisualElement*> elements);
		void spawnProjectiles(VisualElement* element, std::list <VisualElement*> elementsToFocus);


		template <typename T>
		std::list<VisualElement*> convertListToVisualElements(const std::list<T>& elements) {
			static_assert(std::is_base_of<VisualElement, T>::value, "T must be derived from VisualElement");

			std::list<VisualElement*> visualElements;
			for (const auto& element : elements) {
				visualElements.push_back(const_cast<VisualElement*>(static_cast<const VisualElement*>(&element)));
			}
			return visualElements;
		}
	};
}

#endif // GAME_H

