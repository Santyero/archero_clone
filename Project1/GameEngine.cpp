#include <SDL.h>
#include <stdlib.h>
#include <iostream>
#include <list> // Inclua a biblioteca de listas
#include "GameEngine.h"
#include "Window.h"
#include "Player.h"
#include "SDLRendererAdapter.h"
#include "Scene.h"
#include "Projectile.h"
#include "config.h"
#include "Obstacle.h"

namespace Game
{

    GameEngine::GameEngine(Window& window_, RendererPort* rendererPort_, TimeServicePort* timeServicePort_) : window(window_), rendererPort(rendererPort_), timeServicePort(timeServicePort_)
    {
        this->physicsEngine = new PhysicsEngine(this->timeServicePort);
    }

    void GameEngine::startGame()
    {
        SDL_bool done = SDL_FALSE;

        Player player = Player(this->rendererPort, this->physicsEngine, Config::windowWidth / 2 - 25, Config::sceneHeight - 110);
        Scene scene = Scene(this->rendererPort);
        int projectileFramesDelay = 300;

        std::list<Enemy> enemies = this->createEnemies();
        std::list<Obstacle> obstacles = this->createWall();
        // QUESTION : o projetil nao deveria ser do player?

        std::list<VisualElement*> elements;
        elements.emplace_back(&player);
        for (Enemy& enemy : enemies) {
            elements.emplace_back(&enemy);
        }
        for (Obstacle& obstacle : obstacles) {
            elements.emplace_back(&obstacle);
        }

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

            if (projectileFramesDelay > 0) {
                projectileFramesDelay--;
            }
            else {
                elements.emplace_back(new Projectile(
                    this->rendererPort, this->physicsEngine, player.getPositionXInMeters() + 25, player.getPositionYInMeters() - 20
                ));
                projectileFramesDelay = 300;
            }

            scene.renderElement();
            player.verifyKeyboardCommands();

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
        int obstacleColumnsQtde = Config::sceneHeight / 50;
        int obstacleRowQtde = Config::sceneWidth / 50;

        std::list<Obstacle> obstacles;
        srand(time(nullptr));

        for (int i = 0; i < obstacleColumnsQtde; ++i) {
            for (int j = 0; j < obstacleRowQtde; ++j) {
                if (j == 0 || i == 0 || i == obstacleColumnsQtde - 1 || j == obstacleRowQtde - 1) {
                    obstacles.emplace_back(this->rendererPort, 50 * j, 50 * i);
                    obstacles.back().renderElement();
                }
            }
        }

        return obstacles;
    }

}
