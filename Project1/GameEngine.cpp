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
        int projectileSpawnInterval = 200;
           
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
            this->enemies.remove_if([](Enemy& enemy) { return enemy.isDeleted(); });
            this->playerProjectiles.remove_if([](Projectile& projectile) { return projectile.isDeleted(); });
            this->enemyProjectiles.remove_if([](Projectile& projectile) { return projectile.isDeleted(); });
            if (projectileSpawnCounter >= projectileSpawnInterval) {
                std::list<VisualElement*> visualEnemies = convertListToVisualElements(this->enemies);
                if (this->player->getState() == State::SHOOTING)
                {
                    this->spawnProjectiles(this->player, visualEnemies, this->playerProjectiles);
                }
                for (Enemy& enemy : this->enemies) {
                    if (enemy.getState() == State::SHOOTING) {
                        std::list<VisualElement*> visualPlayer;
                        visualPlayer.push_back(this->player);
                        this->spawnProjectiles(&enemy, visualPlayer, this->enemyProjectiles);
                    }
                }
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
        //int enemiesCount = 1;

        srand(time(nullptr));
        for (int i = 0; i < enemiesCount; ++i)
        {
            this->enemies.emplace_back(Enemy(this->rendererPort, this->physicsEngine));
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
        this->player->physicsUpdate(this->timeServicePort->getLastCurrentTimeInSeconds() - this->timeServicePort->getLastElapsedTimeInSeconds());
        this->player->renderElement();

        for (Enemy& enemy : this->enemies) {
			enemy.update();
			enemy.physicsUpdate(this->timeServicePort->getLastCurrentTimeInSeconds() - this->timeServicePort->getLastElapsedTimeInSeconds());
			enemy.renderElement();
		}
        for (Obstacle& obstacle : this->obstacles) {
           obstacle.update();
           obstacle.physicsUpdate(this->timeServicePort->getLastCurrentTimeInSeconds() - this->timeServicePort->getLastElapsedTimeInSeconds());
           obstacle.renderElement();
        }
        for (Projectile& projectile : this->playerProjectiles) {
            projectile.update();
            projectile.physicsUpdate(this->timeServicePort->getLastCurrentTimeInSeconds() - this->timeServicePort->getLastElapsedTimeInSeconds());
            projectile.renderElement();
        }
        for (Projectile& projectile : this->enemyProjectiles) {
            projectile.update();
            projectile.physicsUpdate(this->timeServicePort->getLastCurrentTimeInSeconds() - this->timeServicePort->getLastElapsedTimeInSeconds());
            projectile.renderElement();
        }
	}

    void GameEngine::updateCollisions() {
        for (Projectile& projectile : this->playerProjectiles) {
            for (Obstacle& obstacle : this->obstacles) {
				bool collision = projectile.checkCollision(&obstacle);
                if (collision) {
					projectile.preventTranposition(&obstacle);
					obstacle.preventTranposition(&projectile);
				}
			}
        }
        for (Enemy& enemy : this->enemies) {
            if (this->player->checkCollision(&enemy)) {
                player->preventTranposition(&enemy);
                enemy.preventTranposition(player);
            }
            for (Projectile& projectile : this->playerProjectiles) {
				bool collision = enemy.checkCollision(&projectile);
                if (collision) {
					enemy.preventTranposition(&projectile);
					projectile.preventTranposition(&enemy);
				}
			}
            for (Obstacle& obstacle : this->obstacles) {
				bool collision = enemy.checkCollision(&obstacle);
                if (collision) {
					enemy.preventTranposition(&obstacle);
					obstacle.preventTranposition(&enemy);
				}
                if (this->player->checkCollision(&obstacle)) {
                    player->preventTranposition(&obstacle);
                    obstacle.preventTranposition(player);
                }
			}
		}

        for (Projectile& projectile : this->enemyProjectiles) {
            for (Obstacle& obstacle : this->obstacles) {
                bool collision = projectile.checkCollision(&obstacle);
                if (collision) {
                    projectile.preventTranposition(&obstacle);
                    obstacle.preventTranposition(&projectile);
                }
            }
            if (this->player->checkCollision(&projectile)) {
				player->preventTranposition(&projectile);
				projectile.preventTranposition(player);
			}
        }
        this->loadElements();
    }

    void GameEngine::spawnProjectiles(VisualElement* selectedElement, std::list <VisualElement*> elementsToFocus, std::list<Projectile>& projectileList) {
        VisualElement* targetEnemy = this->findNextElement(selectedElement, elementsToFocus);
        if (targetEnemy == nullptr) return;

        Vector playerPosition = selectedElement->getPosition();
        Vector enemyPosition = targetEnemy->getPosition();
        Vector SelectedElementSize = selectedElement->getSize();
        float SelectedElementSizeX = SelectedElementSize.x;
        float SelectedElementSizeY = SelectedElementSize.y;

        // Calcula a direção do projétil
        Vector direction = enemyPosition - playerPosition;
        direction.set_length(0.5); // Define a velocidade do projétil

        // Define a posição inicial do projétil no centro do jogador
        Vector projectilePosition = { playerPosition.x + SelectedElementSizeX / 2, playerPosition.y + SelectedElementSizeY / 2 };

        // Ajusta a posição inicial do projétil baseado na direção
        if (direction.x != 0 || direction.y != 0) {
            float angle = atan2(direction.y, direction.x);
            projectilePosition.x += cos(angle) * (SelectedElementSizeX / 2 + 1);
            projectilePosition.y += sin(angle) * (SelectedElementSizeY / 2 + 1);
        }

        Vector projectileSize = { 10, 10 };
        projectileList.emplace_back(Projectile(
            this->rendererPort,
            this->physicsEngine,
            projectilePosition,
            projectileSize,
            direction,
            10
        ));
    }



    VisualElement* GameEngine::findNextElement(VisualElement* selectedElement, std::list<VisualElement*> elementsToFind)
    {
        if (elementsToFind.empty() || !selectedElement) {
            return nullptr;
        }

        VisualElement* closestElement = nullptr;
        float minDistance = std::numeric_limits<float>::max();
        auto selectedPosition = selectedElement->getPosition();

        for (auto& elementToVerify : elementsToFind) {
            auto elementPosition = elementToVerify->getPosition();
            float distance = std::sqrt(std::pow(elementPosition.x - selectedPosition.x, 2) +
                std::pow(elementPosition.y - selectedPosition.y, 2));

            if (distance < minDistance) {
                minDistance = distance;
                closestElement = elementToVerify;
            }
        }

        return closestElement;
            
	}

}
