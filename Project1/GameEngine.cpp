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
        int projectileSpawnCounter = 0;
        int projectileSpawnInterval = 50;
           
        this->createEnemies();
        this->createWall();

        this->timeServicePort->updateLastCurrentTimeInSeconds();
        this->timeServicePort->updateLastElapsedTimeInSeconds();

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
            this->timeServicePort->updateLastCurrentTimeInSeconds();
            
            scene.renderElement();

            this->player->verifyKeyboardCommands();

            this->updateCollisions();

            this->rendererPort->renderPresent();
            this->timeServicePort->updateLastElapsedTimeInSeconds();
            this->player->projectiles.remove_if([](Projectile& projectile) { return projectile.isDeleted(); });
            this->enemies.remove_if([](Enemy& enemy) { return enemy.isDeleted(); });
            this->projectiles.remove_if([](Projectile& projectile) { return projectile.isDeleted(); });
            if (projectileSpawnCounter >= projectileSpawnInterval) {
                std::list<VisualElement*> visualEnemies = convertListToVisualElements(this->enemies);
                this->spawnProjectiles(this->player, visualEnemies);
                projectileSpawnCounter = 0; // Reset the counter
            }
            else {
                projectileSpawnCounter++;
            }
        }

        this->rendererPort->destroy();
    }

    void GameEngine::createEnemies()
    {
        int enemiesCount = 1 + (rand() / 8);

        srand(time(nullptr));
        for (int i = 0; i < enemiesCount; ++i)
        {
            this->enemies.emplace_back(this->rendererPort, this->physicsEngine);
            this->enemies.back().renderElement();
        }
    }

    void GameEngine::createWall() {
        int obstacleColumnsQtde = Config::sceneSize.y / 50;
        int obstacleRowQtde = Config::sceneSize.x / 50;

        srand(time(nullptr));

        for (int i = 0; i < obstacleColumnsQtde; ++i) {
            for (int j = 0; j < obstacleRowQtde; ++j) {
                if (j == 0 || i == 0 || i == obstacleColumnsQtde - 1 || j == obstacleRowQtde - 1) {
                    this->obstacles.emplace_back(Obstacle(this->rendererPort, {float(50 * j), float(50 * i)}, {50, 50}));
                    this->obstacles.back().renderElement();
                }
            }
        }
    }

    void GameEngine::loadElements()
    {
		this->player->update();
        this->enemies.begin()->update();
        this->obstacles.begin()->update();
        this->projectiles.begin()->update();
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
        for (Projectile& projectile : this->projectiles) {
            elements.push_back(&projectile);
        }

        int n = elements.size();
        for (auto it1 = elements.begin(); it1 != elements.end(); ++it1) {
            VisualElement* element = *it1;

            // Verifica colisões com elementos subsequentes
            for (auto it2 = std::next(it1); it2 != elements.end(); ++it2) {
                VisualElement* otherElement = *it2;
                bool collision = element->checkCollision(otherElement);
                if (collision) {
                   element->preventTranposition(otherElement);
                   //otherElement->preventTranposition(element);
                };
            }

            // Renderiza o elemento se não estiver marcado para exclusão
            if (!element->isDeleted()) {
                element->update();
                element->physicsUpdate(this->timeServicePort->getLastCurrentTimeInSeconds() - this->timeServicePort->getLastElapsedTimeInSeconds());
                element->renderElement();
            }
        }
        elements.remove_if([](VisualElement* element) { return element->isDeleted(); });
    }

    void GameEngine::spawnProjectiles(VisualElement* selectedElement, std::list <VisualElement*> elementsToFocus) {
        std::cout << "spawnProjectiles" << std::endl;
        VisualElement* inimigo = this->findNextElement(selectedElement, elementsToFocus);
        if (inimigo == nullptr) return;
        Vector projectVelocity = inimigo->getPosition() - selectedElement->getPosition();
        projectVelocity.set_length(0.5); // seta a velocidade do projetil


        Vector projectilePosition = { selectedElement->getPosition().x + 25, selectedElement->getPosition().y - 20 };
        Vector projectileSize = { 10, 10 };
        this->projectiles.emplace_back(Projectile(
            this->rendererPort,
            this->physicsEngine,
            projectilePosition,
            projectileSize,
            projectVelocity,
            10
        ));
        this->projectiles.back().renderElement();
    }

    VisualElement* GameEngine::findNextElement(VisualElement* selectedElement, std::list<VisualElement*> elements)
    {
        if (elements.empty() || !selectedElement) {
            return nullptr;
        }

        VisualElement* closestElement = nullptr;
        float minDistance = std::numeric_limits<float>::max();
        auto selectedPosition = selectedElement->getPosition();
        for (auto& element : elements) {

            auto elementPosition = element->getPosition();
            float distance = std::sqrt(std::pow(elementPosition.x - selectedPosition.x, 2) +
                std::pow(elementPosition.y - selectedPosition.y, 2));

            if (distance < minDistance) {
                minDistance = distance;
                return element;
            }
        }
            
	}

}
