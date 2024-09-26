#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
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
#include "MixerManager.h"


namespace Game
{
    GameEngine::GameEngine(Window& window_, RendererPort* rendererPort_, TimeServicePort* timeServicePort_)
        : window(window_), rendererPort(rendererPort_), timeServicePort(timeServicePort_)
    {
        this->textureManager = std::make_unique<TextureManager>(static_cast<SDLRendererAdapter*>(rendererPort)->getRenderer());
        this->mixerManager = std::make_unique<MixerManager>();
        this->physicsEngine = new PhysicsEngine(this->timeServicePort);
    }

    void GameEngine::startGame()
    {

        SDL_bool done = SDL_FALSE;
		Mix_Init(MIX_INIT_MP3);
        SDL_Init(SDL_INIT_VIDEO);

        if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
            std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
            throw std::runtime_error("Ocorreu um erro!");
        }
        
		this->loadTextures();
        this->loadAudio();

		mixerManager->playMusic("background", -1);

        this->player = new Game::Player(
            this->rendererPort,
            this->textureManager.get(),
            "player",
            this->physicsEngine,
            Vector{ Config::windowSize.x / 2 - 25, Config::sceneSize.y - 110 },
            Vector{ 50, 50 }
        );

        Scene scene(this->rendererPort, this->textureManager.get(), "scene");
        
        int enemyProjectileSpawnCounter = 0;
        int enemySpawnInterval = 1000;
		int plaeyrProjectileSpawnCounter = 0;
        int playerSpawnInterval = 500;
           
        this->createEnemies();
        this->createWall();

        this->timeServicePort->updateLastCurrentTimeInSeconds();
        this->timeServicePort->updateLastElapsedTimeInSeconds();

        this->hud = std::make_unique<HUD>(rendererPort, textureManager.get());


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

			this->verifyRenderNewEnemies();
            
            scene.renderElement();

            this->player->verifyKeyboardCommands();

            this->updateCollisions();


            this->hud->renderElement();
            this->rendererPort->renderPresent();
            this->timeServicePort->updateLastElapsedTimeInSeconds();
            this->enemies.remove_if([](Enemy& enemy) { return enemy.isDeleted(); });
            this->playerProjectiles.remove_if([](Projectile& projectile) { return projectile.isDeleted(); });
            this->enemyProjectiles.remove_if([](Projectile& projectile) { return projectile.isDeleted(); });
            if (enemyProjectileSpawnCounter + enemySpawnInterval <= this->timeServicePort->getCurrentTimeInSeconds()) {
                for (Enemy& enemy : this->enemies) {
                    if (enemy.getVelocity().x == 0 && enemy.getVelocity().y == 0) {
                        std::list<VisualElement*> visualPlayer;
                        visualPlayer.push_back(this->player);
                       this->spawnProjectiles(&enemy, visualPlayer, this->enemyProjectiles, "enemy");
                    }
                }
                enemyProjectileSpawnCounter = this->timeServicePort->getCurrentTimeInSeconds();
            }
			if (plaeyrProjectileSpawnCounter + playerSpawnInterval <= this->timeServicePort->getCurrentTimeInSeconds()) {
                std::list<VisualElement*> visualEnemies = convertListToVisualElements(this->enemies);
                if (this->player->getVelocity().x == 0 && this->player->getVelocity().y == 0)
                {
                    if (visualEnemies.size() <= 0) {
                        this->player->setAnimationState(AnimationState::IDLE);
                    }
                    else {
                        this->spawnProjectiles(this->player, visualEnemies, this->playerProjectiles, "player");
                    }
                    plaeyrProjectileSpawnCounter = this->timeServicePort->getCurrentTimeInSeconds();
                }
			}
        }
        this->rendererPort->destroy();
    }

    void GameEngine::createEnemies()
    {
        int enemiesCount = level;

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
		for (Obstacle& obstacle : this->obstacles) {
			if (this->player->checkCollision(&obstacle)) {
				player->preventTranposition(&obstacle);
				obstacle.preventTranposition(player);
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

    void GameEngine::spawnProjectiles(VisualElement* selectedElement, std::list <VisualElement*> elementsToFocus, std::list<Projectile>& projectileList, const std::string& projectileType) {
        VisualElement* targetEnemy = this->findNextElement(selectedElement, elementsToFocus);
        if (targetEnemy == nullptr) return;

        Vector SelectedElementPosition = selectedElement->getPosition();
        Vector enemyPosition = targetEnemy->getPosition();
        Vector SelectedElementSize = selectedElement->getSize();
        float SelectedElementSizeX = SelectedElementSize.x;
        float SelectedElementSizeY = SelectedElementSize.y;

        Vector direction = enemyPosition - SelectedElementPosition;

        Vector projectilePosition = { SelectedElementPosition.x + SelectedElementSizeX / 2, SelectedElementPosition.y + SelectedElementSizeY / 2 };

        Vector projectileSize;
        int projectileSpeed;
        if (projectileType == "player") {
            mixerManager->playSound("player_shoot");
            projectileSize = { 30, 30 };
            projectileSpeed = 10;
            selectedElement->setAnimationState(AnimationState::SHOOT);
        }
        else if (projectileType == "enemy") {
            mixerManager->playSound("enemy_shoot");
            projectileSize = { 50, 50 };
            projectileSpeed = 7;
            // 70% de chance de atirar na direção do jogador com imprecisão
            if (rand() % 100 < 70) {
                // Adiciona uma pequena imprecisão
                float angle = (rand() % 40 - 20) * 3.14159f / 180.0f; // ângulo entre -20 e 20 graus
                float cosAngle = cos(angle);
                float sinAngle = sin(angle);
                float newX = direction.x * cosAngle - direction.y * sinAngle;
                float newY = direction.x * sinAngle + direction.y * cosAngle;
                direction = Vector(newX, newY);
            }
            else {
                // 30% de chance de atirar em uma direção completamente aleatória
                direction = Vector(rand() % 200 - 100, rand() % 200 - 100);
            }
        }

        direction.set_length(0.5);
        projectileList.emplace_back(this->rendererPort, this->textureManager.get(), projectileType + "_projectile",
            this->physicsEngine, projectilePosition, projectileSize, direction, 10);
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

    void GameEngine::loadAnimationFrames(std::vector<SDL_Rect>& targetFrames, int startY, int numFrames, int frameHeight, int frameWidth) {
        for (size_t i = 0; i < numFrames; i++) {
            int x = 60 + 190 * i;
            targetFrames.push_back({ x, startY, frameHeight, frameWidth });
        }
    }

    void GameEngine::setupPlayerAnimations() {
        AnimationInfo playerAnimInfo;
        loadAnimationFrames(playerAnimInfo.idleFrames, 50, 6, 85, 97);   // Idle
        loadAnimationFrames(playerAnimInfo.walkFrames, 240, 6, 85, 97); // Walk
        loadAnimationFrames(playerAnimInfo.shootTopFrames, 430, 8, 85, 97); // Shoot top
        loadAnimationFrames(playerAnimInfo.shootAngularTopFrames, 820, 8, 85, 97);   // Shoot angular top
        loadAnimationFrames(playerAnimInfo.shootFrame, 1210, 8, 85, 97);  // Shoot
        loadAnimationFrames(playerAnimInfo.shootAngularBottomFrames , 1400, 8, 85, 97);  // Shoot angular bottom
        loadAnimationFrames(playerAnimInfo.shootBottomFrames, 1590, 8, 85, 97);  // Shoot bottom
        textureManager->loadTextures("player", "archer.png", playerAnimInfo);
    }

    void GameEngine::setupEnemyAnimations() {
        AnimationInfo enemyAnimInfo;
        loadAnimationFrames(enemyAnimInfo.idleFrames, 50, 6, 85, 97);     // Idle and Walk
        loadAnimationFrames(enemyAnimInfo.walkFrames, 240, 6, 85, 97);    // Walk
        loadAnimationFrames(enemyAnimInfo.shootFrame, 430, 7, 85, 97);    // Shoot
        textureManager->loadTextures("enemy", "TNT_Red.png", enemyAnimInfo);
    }

    void GameEngine::setupProjectiles() {
        AnimationInfo playerProjectile, enemyProjectile;
        playerProjectile.idleFrames.push_back({ 0, 25, 85, 20 });
		//loadAnimationFrames(enemyProjectile.idleFrames, 50, 6, 85, 97);
		enemyProjectile.idleFrames.push_back({ 50, 6, 85, 97});
        textureManager->loadTextures("player_projectile", "Arrow.png", playerProjectile);
        textureManager->loadTextures("enemy_projectile", "Dynamite.png", enemyProjectile);
    }

    void GameEngine::setutpScene() {
		AnimationInfo sceneAnimInfo;
		sceneAnimInfo.idleFrames.push_back({ 20, 90, 100, 100 });
		textureManager->loadTextures("scene", "scene.png", sceneAnimInfo);

        AnimationInfo obstacleAnimInfo;
        obstacleAnimInfo.idleFrames.push_back({ 10, 100, 100, 100 });
		textureManager->loadTextures("obstacle", "obstacle.png", obstacleAnimInfo);
    }

    void GameEngine::loadTextures() {
        setupPlayerAnimations();
        setupEnemyAnimations();
        setupProjectiles();
		setutpScene();

        AnimationInfo bannerInfo;
        bannerInfo.idleFrames.push_back({ 0, 0, 256, 128 });
        if (!textureManager->loadTextures("hud_banner", "banner_horizontal.png", bannerInfo)) {
            std::cerr << "Falha ao carregar a textura do banner do HUD" << std::endl;
        }
    }

    void GameEngine::loadAudio() {
        mixerManager->loadMusic("background", "music.mp3");
        mixerManager->loadSound("player_shoot", "bow_shoot.wav");
        mixerManager->loadSound("enemy_shoot", "throw.wav");
        //mixerManager->loadSound("enemy_hit", "path/to/enemy_hit.wav");
    }

    GameEngine::~GameEngine() {
        // Limpeza de recursos, se necessário
        delete physicsEngine;
        delete player;
        // Não é necessário deletar rendererPort e timeServicePort se eles são gerenciados externamente
    }

	void GameEngine::changeLevel() {
		level++;

		createEnemies();
		this->player->setLife(100);
        this->hud->setLevel(level);
	}

	void GameEngine::verifyRenderNewEnemies() {
        if (this->enemies.empty()) {
            if (timerChangeLevel == 0) {
                timerChangeLevel = this->timeServicePort->getCurrentTimeInSeconds();
            }
            else if (timerChangeLevel + 10000 <= this->timeServicePort->getCurrentTimeInSeconds()) {
                this->changeLevel();
                timerChangeLevel = 0;
            }
        }
	}
}
