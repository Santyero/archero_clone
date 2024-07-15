#include <SDL.h>
#include <stdlib.h>
#include <iostream>
#include <list>
#include "GameEngine.h"
#include "Window.h"
#include "Player.h"
#include "SDLRendererAdapter.h"
#include "Scene.h"
#include "Projectile.h"
#include "config.h"

namespace Game
{

    GameEngine::GameEngine(Window& window_, RendererPort* rendererPort_, TimeServicePort* timeServicePort_) : window(window_), rendererPort(rendererPort_), timeServicePort(timeServicePort_)
    {
        this->physicsEngine = new PhysicsEngine(this->timeServicePort);  
    }

    void GameEngine::startGame()
    {
        SDL_bool done = SDL_FALSE;

        this->player = new Game::Player(this->rendererPort, this->physicsEngine, { Config::windowSize.x / 2 - 25, Config::sceneSize.y - 110 }, { 50,50 });
        Scene scene = Scene(this->rendererPort);
           
        this->enemies.emplace_back(this->createEnemies());
        this->obstacles.emplace_back(this->createWall());

        /*std::list<VisualElement*> elements;
        elements.emplace_back(&player);
        for (Enemy& enemy : enemies) {
            elements.emplace_back(&enemy);
        }
        for (Obstacle& obstacle : obstacles) {
            elements.emplace_back(&obstacle);
        }*/

        this->timeServicePort->updateLastCurrentTimeInMilliseconds();
        this->timeServicePort->updateLastElapsedTimeInMilliseconds();

        while (!done)
        {
            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                {
                    done = SDL_TRUE;
                }
            }
            this->timeServicePort->updateLastCurrentTimeInMilliseconds();

            // this->player.renderProjects();
            scene.renderElement();

            this->player->verifyKeyboardCommands();

            this->updateCollisions();

            this->rendererPort->renderPresent();
            this->timeServicePort->updateLastElapsedTimeInMilliseconds();
        }

        this->rendererPort->destroy();
    }

    std::list<Enemy> GameEngine::createEnemies()
    {
        int enemiesCount = 1 + (rand() / 8);

        std::list<Enemy> enemies;
        srand(time(nullptr));
        for (int i = 0; i < enemiesCount; ++i)
        {
            enemies.emplace_back(this->rendererPort, this->physicsEngine);
            enemies.back().renderElement();
        }

        return enemies;
    }

    std::list<Obstacle> GameEngine::createWall() {
        int obstacleColumnsQtde = Config::sceneSize.y / 50;
        int obstacleRowQtde = Config::sceneSize.x / 50;

        std::list<Obstacle> obstacles;
        srand(time(nullptr));

        for (int i = 0; i < obstacleColumnsQtde; ++i) {
            for (int j = 0; j < obstacleRowQtde; ++j) {
                if (j == 0 || i == 0 || i == obstacleColumnsQtde - 1 || j == obstacleRowQtde - 1) {
                    obstacles.emplace_back(Obstacle(this->rendererPort, {float(50 * j), float(50 * i)}, {50, 50}));
                    obstacles.back().renderElement();
                }
            }
        }

        return obstacles;
    }

    void GameEngine::loadElements()
    {
		this->player->update();
        this->enemies.begin()->update();
        this->obstacles.begin()->update();
        this->player->projectiles.begin()->update();
	}

    void GameEngine::updateCollisions() {
        std::list<VisualElement*> elements;
        elements.push_back(this->player);
        for (Enemy& enemy : this->enemies) {
            elements.push_back(&enemy);
        }
        for (Obstacle& obstacle : this->obstacles) {
            elements.push_back(&obstacle);
        }
        for (Projectile& projectile : this->player->projectiles) {
            elements.push_back(&projectile);
        }

        int n = elements.size();
        for (auto it1 = elements.begin(); it1 != elements.end(); ++it1) {
            VisualElement* element = *it1;

            // Verifica colisões com elementos subsequentes
            for (auto it2 = std::next(it1); it2 != elements.end(); ++it2) {
                VisualElement* otherElement = *it2;
                element->checkCollision(otherElement);
                otherElement->checkCollision(element);
            }

            // Renderiza o elemento se não estiver marcado para exclusão
            if (!element->isDeleted()) {
                element->update();
                element->renderElement();
            }
        }
        elements.remove_if([](VisualElement* element) { return element->isDeleted(); });
    }

}
