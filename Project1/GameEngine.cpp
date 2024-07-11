#include <SDL.h>
#include <stdlib.h>
#include <iostream>
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

    GameEngine::GameEngine(Window &window_, RendererPort *rendererPort_, TimeServicePort *timeServicePort_) : window(window_), rendererPort(rendererPort_), timeServicePort(timeServicePort_)
    {
        this->physicsEngine = new PhysicsEngine(this->timeServicePort);
    }

    void GameEngine::startGame()
    {
        SDL_bool done = SDL_FALSE;

        Player player = Player(this->rendererPort, this->physicsEngine, Config::windowWidth / 2 - 25, Config::sceneHeight - 110);
        Scene scene = Scene(this->rendererPort);
        int projectileFramesDelay = 300;

        std::vector<Enemy> enemies = this->createEnemies();
        std::vector<Obstacle> obstacles = this->createWall();
        // QUESTION : o projetil nao deveria ser do player?

        std::vector<VisualElement *> elements;
        elements.push_back(&player);
        for (Enemy &enemy : enemies) {
            elements.push_back(&enemy);
        }
        for (Obstacle &obstacle : obstacles) {
            elements.push_back(&obstacle);
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
                Projectile* projectile = new Projectile(
                    this->rendererPort, this->physicsEngine, player.getPositionXInMeters() + 25, player.getPositionYInMeters() - 20
                );
                elements.push_back(projectile);
                projectileFramesDelay = 300;
            }


            scene.renderElement();
            player.verifyKeyboardCommands();

            int n = elements.size();
            for (int i = 0; i < n; ++i) {
                VisualElement* element = elements[i];

                // Verifica colis�es com elementos subsequentes
                for (int j = i + 1; j < n; ++j) {
                    VisualElement* otherElement = elements[j];
                    element->checkCollision(otherElement);
                    otherElement->checkCollision(element);
                }

                // Renderiza o elemento se n�o estiver marcado para exclus�o
                if (!element->isDeleted()) {
                    element->update();
                    element->renderElement();
                }
            }

            elements.erase(std::remove_if(elements.begin(), elements.end(),
                [](VisualElement* element) { return element->isDeleted(); }),
                elements.end());


            this->rendererPort->renderPresent();
            this->timeServicePort->updateLastElapsedTimeInMilliseconds();
        }

        this->rendererPort->destroy();
    }

    std::vector<Enemy> GameEngine::createEnemies()
    {
        int enemiesCount = 1 + (rand() / 8);

        std::vector<Enemy> enemies;
        srand(time(nullptr));
        for (int i = 0; i < enemiesCount; ++i)
        {
            Enemy enemy = Enemy(this->rendererPort, this->physicsEngine);
            enemies.push_back(enemy);
            enemy.renderElement();
        }

        return enemies;
    }

    std::vector<Obstacle> GameEngine::createWall() {
        int obstacleColumnsQtde = Config::sceneHeight / 50;
        int obstacleRowQtde = Config::sceneWidth / 50;

        std::vector<Obstacle> obstacles;
        srand(time(nullptr));

        for (int i = 0; i < obstacleColumnsQtde; ++i) {
            for (int j = 0; j < obstacleRowQtde; ++j) {
                if (j == 0 || i == 0 || i == obstacleColumnsQtde - 1 || j == obstacleRowQtde - 1) {
                    Obstacle obstacle = Obstacle(this->rendererPort, 50 * j, 50 * i);
                    obstacle.renderElement();
                    obstacles.push_back(obstacle);
                }
            }
        }

        return obstacles;
    }

}