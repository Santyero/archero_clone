#pragma once
#ifndef GAME_H
#define GAME_H

#include "Window.h"
#include "Enemy.h"
#include "TimeServicePort.h"
#include "RendererPort.h"
#include "PhysicsEngine.h"
#include "Obstacle.h"
#include <vector>

namespace Game {
	class GameEngine
	{
	private:
		Window& window;
		RendererPort* rendererPort = nullptr;
		TimeServicePort* timeServicePort = nullptr;
		PhysicsEngine* physicsEngine = nullptr;

		std::vector<Obstacle> createWall();
	public:
		GameEngine(Window& window_, RendererPort* rendererPort_, TimeServicePort* timeServicePort_);

		std::vector<Enemy> createEnemies();
		void startGame();
	};
}

#endif // GAME_H

