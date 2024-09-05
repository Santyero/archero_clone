#include <SDL.h>
#include <SDL_image.h>
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
#include <memory>


namespace Game
{

    GameEngine::GameEngine(Window& window_, RendererPort* rendererPort_, TimeServicePort* timeServicePort_)
        : window(window_), rendererPort(rendererPort_), timeServicePort(timeServicePort_)
    {
        this->textureManager = std::make_unique<TextureManager>(static_cast<SDLRendererAdapter*>(rendererPort)->getRenderer());
        this->physicsEngine = new PhysicsEngine(this->timeServicePort);
    }

    void GameEngine::startGame()
    {
        SDL_bool done = SDL_FALSE;
        SDL_Init(SDL_INIT_VIDEO);

        if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
            std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
            throw std::runtime_error("Ocorreu um erro!");
        }
        
		this->loadTextures();
        AnimationInfo playerAnimInfo;
        playerAnimInfo.idleFrames.resize(textureManager->getFrameCount("player", "idle"));
        playerAnimInfo.runningFrames.resize(textureManager->getFrameCount("player", "running"));
        playerAnimInfo.shootingFrames.resize(textureManager->getFrameCount("player", "shooting"));

        this->player = new Game::Player(
            this->rendererPort,
            this->textureManager.get(),
            "player",
            this->physicsEngine,
            Vector{ Config::windowSize.x / 2 - 25, Config::sceneSize.y - 110 },
            Vector{ 50, 50 }
        );

        Scene scene(this->rendererPort, this->textureManager.get(), "scene");
        
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
                if (this->player->getState() == AnimationState::SHOOTING)
                {
                    this->spawnProjectiles(this->player, visualEnemies, this->playerProjectiles);
                }
                for (Enemy& enemy : this->enemies) {
                    if (enemy.getState() == AnimationState::SHOOTING) {
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

        srand(time(nullptr));
        for (int i = 0; i < enemiesCount; ++i)
        {
            this->enemies.emplace_back(Enemy(this->rendererPort, this->textureManager.get(), "enemy", this->physicsEngine));
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
                    this->obstacles.emplace_back(this->rendererPort, this->textureManager.get(), "obstacle",
                        Vector{ float(50 * j), float(50 * i) }, Vector{ 50, 50 });
                    this->obstacles.back().renderElement();
                }
            }
        }
    }

    void GameEngine::loadElements()
    {
        std::list<VisualElement*> elements;

        elements.push_back(this->player);

        std::list<VisualElement*> visualEnemies = convertListToVisualElements(this->enemies);
        std::list<VisualElement*> visualObstacles = convertListToVisualElements(this->obstacles);
        std::list<VisualElement*> visualPlayerProjectiles = convertListToVisualElements(this->playerProjectiles);
        std::list<VisualElement*> visualEnemyProjectiles = convertListToVisualElements(this->enemyProjectiles);

        elements.insert(elements.end(), visualEnemies.begin(), visualEnemies.end());
        elements.insert(elements.end(), visualObstacles.begin(), visualObstacles.end());
        elements.insert(elements.end(), visualPlayerProjectiles.begin(), visualPlayerProjectiles.end());
        elements.insert(elements.end(), visualEnemyProjectiles.begin(), visualEnemyProjectiles.end());

        for (VisualElement* element : elements) {
            element->update();
            element->physicsUpdate(this->timeServicePort->getLastCurrentTimeInSeconds() - this->timeServicePort->getLastElapsedTimeInSeconds());
            element->renderElement();
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

        Vector SelectedElementPosition = selectedElement->getPosition();
        Vector enemyPosition = targetEnemy->getPosition();
        Vector SelectedElementSize = selectedElement->getSize();
        float SelectedElementSizeX = SelectedElementSize.x;
        float SelectedElementSizeY = SelectedElementSize.y;

        Vector direction = enemyPosition - SelectedElementPosition;

        Vector projectilePosition = { SelectedElementPosition.x + SelectedElementSizeX / 2, SelectedElementPosition.y + SelectedElementSizeY / 2 };

        direction.set_length(0.5);
        Vector projectileSize = { 10, 10 };
        projectileList.emplace_back(this->rendererPort, this->textureManager.get(), "projectile",
            this->physicsEngine, projectilePosition, projectileSize, direction, 10);
        //std::cout << "<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
        //std::cout << "INIMIGO" << enemyPosition << std::endl;
        //std::cout << "DIRECAO" << direction << std::endl;
        //std::cout << "POSICAO" << projectilePosition << std::endl;
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

    void GameEngine::loadTextures()
    {
        AnimationInfo playerAnimInfo;
        playerAnimInfo.idleFrames = {
            {0, 0, 64, 64},
            {64, 0, 64, 64}
        };
        playerAnimInfo.runningFrames = {
            {0, 64, 64, 64},
            {64, 64, 64, 64},
            {128, 64, 64, 64},
            {192, 64, 64, 64},
            {256, 64, 64, 64},
            {320, 64, 64, 64},
            {384, 64, 64, 64},
            {448, 64, 64, 64}
        };
        playerAnimInfo.shootingFrames = {
            {0, 128, 64, 64},
            {64, 128, 64, 64}
        };

        textureManager->loadTextures("player", "Idle_and_running.png", playerAnimInfo);
    }


}
