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
		Player* player = nullptr;

		
	public:
		GameEngine(Window& window_, RendererPort* rendererPort_, TimeServicePort* timeServicePort_);

		void createWall();
		void createEnemies();

		void startGame();
		void loadElements();
		void updateCollisions();
	};
}

#endif // GAME_H

