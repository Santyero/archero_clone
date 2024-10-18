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
#include "ScoreManager.h"
#include "PauseMenu.h"
#include "GameStateManager.h"


namespace Game
{
    GameEngine::GameEngine(Window& window_, RendererPort* rendererPort_, TimeServicePort* timeServicePort_)
        : window(window_), rendererPort(rendererPort_), timeServicePort(timeServicePort_)
    {
        this->textureManager = std::make_unique<TextureManager>(static_cast<SDLRendererAdapter*>(rendererPort)->getRenderer());
        this->mixerManager = std::make_unique<MixerManager>();
        this->physicsEngine = new PhysicsEngine(this->timeServicePort);
        optionsMenu = std::make_unique<OptionsMenu>(rendererPort, textureManager.get(), mixerManager.get(), window.getIsFullscreen());


        if (TTF_Init() == -1) {
            std::cerr << "SDL_ttf não pôde ser inicializado! Erro SDL_ttf: " << TTF_GetError() << std::endl;
            throw std::runtime_error("Falha ao inicializar SDL_ttf");
        }

        font = TTF_OpenFont("font.ttf", 24);
        if (!font) {
            std::cerr << "Falha ao carregar a fonte! Erro SDL_ttf: " << TTF_GetError() << std::endl;
            throw std::runtime_error("Falha ao carregar a fonte");
        }
    }

    void GameEngine::run() {
        SDL_bool done = SDL_FALSE;
        currentState = GameState::MainMenu;

        // Inicialização
        Mix_Init(MIX_INIT_MP3);
        SDL_Init(SDL_INIT_VIDEO);

        if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
            std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
            throw std::runtime_error("Ocorreu um erro!");
        }

        this->loadTextures();
        this->loadAudio();

        this->mainMenu = std::make_unique<MainMenu>(rendererPort, textureManager.get());

        while (!done) {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    done = SDL_TRUE;
                    return;
                }

                switch (currentState) {
                case GameState::MainMenu:
                    startMenu(event);
                    break;
                case GameState::Playing:
                    GameStateManager::getInstance()->setPaused(false);
                    startGame();
                    // Após startGame(), o estado pode ter mudado para MainMenu (após Game Over)
                    break;
                case GameState::Paused:
                    // Lógica de pausa, se necessário
                    break;
				case GameState::Exit:
					done = SDL_TRUE;
                    break;
                }

            }
            this->rendererPort->renderPresent();
        }

        this->rendererPort->destroy();
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
        int playerProjectileSpawnCounter = 0;
        int playerSpawnInterval = 500;

        this->createEnemies();
        this->createWall();

        this->timeServicePort->updateLastCurrentTimeInSeconds();
        this->timeServicePort->updateLastElapsedTimeInSeconds();

        this->hud = std::make_unique<HUD>(rendererPort, textureManager.get());

        GameStateManager* gameState = GameStateManager::getInstance();
        PauseMenu pauseMenu(this->rendererPort, std::make_unique<TextureManager>(*textureManager));
        bool isPaused = false;

        while (!done)
        {
            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                {
                    done = SDL_TRUE;
                    this->currentState = GameState::Exit;
                }
                else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
                {
                    gameState->setPaused(!gameState->getIsPaused());
                    if (gameState->getIsPaused()) {
                        mixerManager->pauseMusic();
                        this->timeServicePort->pauseTime();
                    }
                    else {
                        mixerManager->resumeMusic();
                        this->timeServicePort->resumeTime();
                    }
                }

                if (gameState->getIsPaused())
                {
                    pauseMenu.handleInput(event);

                    if (event.type == SDL_MOUSEBUTTONDOWN)
                    {
                        int mouseX, mouseY;
                        SDL_GetMouseState(&mouseX, &mouseY);
                        int clickedOption = pauseMenu.handleMouseClick(mouseX, mouseY);
                        if (clickedOption != -1)
                        {
                            handlePauseMenuSelection(clickedOption,  done);
                        }
                    }
                    else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN)
                    {
                        int selectedOption = pauseMenu.getSelectedOption();
                        handlePauseMenuSelection(selectedOption,  done);
                    }
                }
                else
                {
                    // Processa inputs do jogo apenas quando não estiver pausado
                    this->player->verifyKeyboardCommands();
                }
            }

            if (!gameState->getIsPaused())
            {
                this->timeServicePort->updateLastCurrentTimeInSeconds();

                this->verifyRenderNewEnemies();

                this->updateCollisions();

                if (player->getLife() <= 0) {
                    handlePlayerDeath();
                    done = SDL_TRUE;
                    break;
                }

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

                if (playerProjectileSpawnCounter + playerSpawnInterval <= this->timeServicePort->getCurrentTimeInSeconds()) {
                    std::list<VisualElement*> visualEnemies = convertListToVisualElements(this->enemies);
                    if (this->player->getVelocity().x == 0 && this->player->getVelocity().y == 0)
                    {
                        if (visualEnemies.empty()) {
                            this->player->setAnimationState(AnimationState::IDLE);
                        }
                        else {
                            this->spawnProjectiles(this->player, visualEnemies, this->playerProjectiles, "player");
                        }
                        playerProjectileSpawnCounter = this->timeServicePort->getCurrentTimeInSeconds();
                    }
                }

                this->timeServicePort->updateLastElapsedTimeInSeconds();

                // Renderiza o jogo
                scene.renderElement();
                this->player->renderElement();
                for (auto& enemy : this->enemies) {
                    enemy.renderElement();
                }
                for (auto& projectile : this->playerProjectiles) {
                    projectile.renderElement();
                }
                for (auto& projectile : this->enemyProjectiles) {
                    projectile.renderElement();
                }
                for (auto& obstacle : this->obstacles) {
                    obstacle.renderElement();
                }
                this->hud->update();
                this->hud->renderElement();
            }
            else
            {
                // Renderiza apenas o menu de pausa
                pauseMenu.renderElement();
				player->stop();
                for (auto& enemy : this->enemies) {
                    enemy.stop();
                }
            }

            this->rendererPort->renderPresent();
        }

        this->resetGame();
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
        bannerInfo.idleFrames.push_back({ 30, 40, 256, 128 });
        if (!textureManager->loadTextures("hud_banner", "banner_horizontal.png", bannerInfo)) {
            std::cerr << "Falha ao carregar a textura do banner do HUD" << std::endl;
        }

		AnimationInfo hudPointsInfo;
		hudPointsInfo.idleFrames.push_back({ 30, 40, 256, 128 });
        if (!textureManager->loadTextures("hud_points", "Banner_Connection_Left.png", hudPointsInfo)) {
            std::cerr << "Falha ao carregar a textura do banner do HUD" << std::endl;
        }

        AnimationInfo mainMenuInfo;
        mainMenuInfo.idleFrames.push_back({ 0, 0, static_cast<int>(Config::windowSize.x / 1.5), static_cast<int>(Config::windowSize.y / 1.5)  });
        if (!textureManager->loadTextures("main_menu_background", "main_menu_background.png", mainMenuInfo)) {
            std::cerr << "Falha ao carregar a textura de fundo do menu principal" << std::endl;
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
        ScoreManager::getInstance()->setLevel(level);
	}

	void GameEngine::verifyRenderNewEnemies() {
        if (this->enemies.empty()) {
            if (timerChangeLevel == 0) {
                timerChangeLevel = this->timeServicePort->getCurrentTimeInSeconds();
            }
            else if (timerChangeLevel + 5000 <= this->timeServicePort->getCurrentTimeInSeconds()) {
                this->changeLevel();
                timerChangeLevel = 0;
            }
        }
	}

    void GameEngine::handlePauseMenuSelection(int selectedOption, SDL_bool& done)
    {
        GameStateManager* gameState = GameStateManager::getInstance();
        switch (selectedOption)
        {
        case 0: // Resume
            gameState->setPaused(false);
            mixerManager->resumeMusic();
            break;
        case 1: // Options
            showOptionsMenu();
            break;
        case 2: // Quit
            done = SDL_TRUE;
            this->currentState = GameState::MainMenu;
            break;
        }
    }

    void GameEngine::startMenu(SDL_Event& event) {
        this->mainMenu->handleInput(event);
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
            int selectedOption = mainMenu->getSelectedOption();
            switch (selectedOption) {
            case 0: // New Game
                currentState = GameState::Playing;
                break;
            case 1: // Options
                showOptionsMenu();
                break;
            case 2: // High Scores
                showHighScores();
                break;
            case 3: // Credits
                showCredits();
                break;
            case 4: // Exit
                currentState = GameState::Exit;
                break;
            }
        }

        this->mainMenu->render();
    }

    void GameEngine::handlePlayerDeath() {
        std::string playerName = getPlayerName();
        ScoreManager::getInstance()->saveScore(playerName);

        // Mostrar tela de game over
        showGameOverScreen();
    }

    std::string GameEngine::getPlayerName() {
        std::string playerName;
        SDL_StartTextInput();

        SDL_Event event;
        bool done = false;

        while (!done) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    done = true;
                }
                else if (event.type == SDL_KEYDOWN) {
                    if (event.key.keysym.sym == SDLK_RETURN && !playerName.empty()) {
                        done = true;
                    }
                    else if (event.key.keysym.sym == SDLK_BACKSPACE && !playerName.empty()) {
                        playerName.pop_back();
                    }
                }
                else if (event.type == SDL_TEXTINPUT && playerName.length() < 7) {
                    playerName += event.text.text;
                }
            }

            renderNameInputScreen(playerName);
        }

        SDL_StopTextInput();
        return playerName;
    }

    void GameEngine::renderNameInputScreen(const std::string& playerName) {
        // Limitar o nome do jogador a 7 caracteres
        std::string displayName = playerName.substr(0, 7);

        // Se o nome estiver vazio, use um placeholder
        if (displayName.empty()) {
            displayName = "_";
        }

        // Limpar a tela
        SDL_SetRenderDrawColor(static_cast<SDLRendererAdapter*>(rendererPort)->getRenderer(), 0, 0, 0, 255);
        SDL_RenderClear(static_cast<SDLRendererAdapter*>(rendererPort)->getRenderer());

        // Verificar se a fonte foi carregada
        if (!font) {
            std::cerr << "Erro: Fonte não carregada" << std::endl;
            return;
        }

        // Renderizar o texto "Enter your name:"
        SDL_Color textColor = { 255, 255, 255, 255 };
        SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, "Enter your name:", textColor);
        if (!surfaceMessage) {
            std::cerr << "Erro ao renderizar texto: " << TTF_GetError() << std::endl;
            return;
        }

        SDL_Texture* message = SDL_CreateTextureFromSurface(static_cast<SDLRendererAdapter*>(rendererPort)->getRenderer(), surfaceMessage);
        if (!message) {
            std::cerr << "Erro ao criar textura: " << SDL_GetError() << std::endl;
            SDL_FreeSurface(surfaceMessage);
            return;
        }

        int textWidth = surfaceMessage->w;
        int textHeight = surfaceMessage->h;
        SDL_Rect messageRect = { (Config::windowSize.x - textWidth) / 2, Config::windowSize.y / 3, textWidth, textHeight };
        SDL_RenderCopy(static_cast<SDLRendererAdapter*>(rendererPort)->getRenderer(), message, NULL, &messageRect);

        // Renderizar o nome do jogador
        SDL_Surface* nameSurface = TTF_RenderText_Solid(font, displayName.c_str(), textColor);
        if (!nameSurface) {
            std::cerr << "Erro ao renderizar nome do jogador: " << TTF_GetError() << std::endl;
            SDL_DestroyTexture(message);
            SDL_FreeSurface(surfaceMessage);
            return;
        }

        SDL_Texture* nameTexture = SDL_CreateTextureFromSurface(static_cast<SDLRendererAdapter*>(rendererPort)->getRenderer(), nameSurface);
        if (!nameTexture) {
            std::cerr << "Erro ao criar textura do nome: " << SDL_GetError() << std::endl;
            SDL_FreeSurface(nameSurface);
            SDL_DestroyTexture(message);
            SDL_FreeSurface(surfaceMessage);
            return;
        }

        int nameWidth = nameSurface->w;
        int nameHeight = nameSurface->h;
        SDL_Rect nameRect = { (Config::windowSize.x - nameWidth) / 2, Config::windowSize.y / 2, nameWidth, nameHeight };
        SDL_RenderCopy(static_cast<SDLRendererAdapter*>(rendererPort)->getRenderer(), nameTexture, NULL, &nameRect);

        // Liberar recursos
        SDL_FreeSurface(surfaceMessage);
        SDL_FreeSurface(nameSurface);
        SDL_DestroyTexture(message);
        SDL_DestroyTexture(nameTexture);

        // Apresentar a renderização
        SDL_RenderPresent(static_cast<SDLRendererAdapter*>(rendererPort)->getRenderer());
    }

    void GameEngine::showGameOverScreen() {
        // Renderizar a tela de game over
        SDL_SetRenderDrawColor(static_cast<SDLRendererAdapter*>(rendererPort)->getRenderer(), 0, 0, 0, 255);
        SDL_RenderClear(static_cast<SDLRendererAdapter*>(rendererPort)->getRenderer());

        SDL_Color textColor = { 255, 255, 255, 255 };

        // Renderizar "Game Over"
        SDL_Surface* gameOverSurface = TTF_RenderText_Solid(font, "Game Over", textColor);
        SDL_Texture* gameOverTexture = SDL_CreateTextureFromSurface(static_cast<SDLRendererAdapter*>(rendererPort)->getRenderer(), gameOverSurface);

        int gameOverWidth = gameOverSurface->w;
        int gameOverHeight = gameOverSurface->h;
        SDL_Rect gameOverRect = { (Config::windowSize.x - gameOverWidth) / 2, Config::windowSize.y / 3, gameOverWidth, gameOverHeight };
        SDL_RenderCopy(static_cast<SDLRendererAdapter*>(rendererPort)->getRenderer(), gameOverTexture, NULL, &gameOverRect);

        // Renderizar a pontuação
        int score = ScoreManager::getInstance()->getPoints();
        std::string scoreText = "Score: " + std::to_string(score);
        SDL_Surface* scoreSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
        SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(static_cast<SDLRendererAdapter*>(rendererPort)->getRenderer(), scoreSurface);

        int scoreWidth = scoreSurface->w;
        int scoreHeight = scoreSurface->h;
        SDL_Rect scoreRect = { (Config::windowSize.x - scoreWidth) / 2, Config::windowSize.y / 2, scoreWidth, scoreHeight };
        SDL_RenderCopy(static_cast<SDLRendererAdapter*>(rendererPort)->getRenderer(), scoreTexture, NULL, &scoreRect);

        rendererPort->renderPresent();

        // Esperar alguns segundos antes de voltar ao menu principal
        SDL_Delay(1000);

        // Liberar recursos
        SDL_FreeSurface(gameOverSurface);
        SDL_DestroyTexture(gameOverTexture);
        SDL_FreeSurface(scoreSurface);
        SDL_DestroyTexture(scoreTexture);


        // Voltar ao menu principal
        this->currentState = GameState::MainMenu;
    }

    void GameEngine::resetGame() {
        // Limpar listas de inimigos, projéteis, etc.
        enemies.clear();
        playerProjectiles.clear();
        enemyProjectiles.clear();
        obstacles.clear();

        // Resetar o jogador
        if (player) {
            delete player;
            player = nullptr;
        }

        // Resetar nível e pontuação
        level = 1;
        timerChangeLevel = 0;
        ScoreManager::getInstance()->resetPoints();

        // Resetar outros estados do jogo conforme necessário
    }

    void GameEngine::showHighScores() {
        bool backToMenu = false;
        std::vector<PlayerScore> highScores = ScoreManager::getInstance()->getHighScores();

        while (!backToMenu) {
            // Limpar a tela
            SDL_SetRenderDrawColor(static_cast<SDLRendererAdapter*>(rendererPort)->getRenderer(), 0, 0, 0, 255);
            SDL_RenderClear(static_cast<SDLRendererAdapter*>(rendererPort)->getRenderer());

            // Renderizar o título
            SDL_Color titleColor = { 255, 255, 0, 255 }; // Amarelo
            rendererPort->renderSimpleText("High Scores", Config::windowSize.x / 2 - 100, 50, titleColor);

            // Renderizar as pontuações
            SDL_Color textColor = { 255, 255, 255, 255 }; // Branco
            int yPos = 100;
            for (size_t i = 0; i < std::min(highScores.size(), size_t(10)); ++i) {
                std::string scoreText = std::to_string(i + 1) + ". " + highScores[i].name + ": " + std::to_string(highScores[i].score);
                rendererPort->renderSimpleText(scoreText, Config::windowSize.x / 2 - 150, yPos, textColor);
                yPos += 40;
            }

            // Renderizar instrução para voltar
            SDL_Color instructionColor = { 150, 150, 150, 255 }; // Cinza
            rendererPort->renderSimpleText("Press ESC to return to main menu", Config::windowSize.x / 2 - 150, Config::windowSize.y - 50, instructionColor);

            rendererPort->renderPresent();

            // Tratamento de eventos
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    backToMenu = true;
                }
                else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                    backToMenu = true;
                }
            }
        }
    }

    void GameEngine::showCredits() {
        bool backToMenu = false;

        while (!backToMenu) {
            // Limpar a tela
            SDL_SetRenderDrawColor(static_cast<SDLRendererAdapter*>(rendererPort)->getRenderer(), 0, 0, 0, 255);
            SDL_RenderClear(static_cast<SDLRendererAdapter*>(rendererPort)->getRenderer());

            // Renderizar o título
            SDL_Color titleColor = { 255, 255, 0, 255 }; // Amarelo
            rendererPort->renderSimpleText("Credits", Config::windowSize.x / 2 - 50, 50, titleColor);

            // Renderizar os créditos
            SDL_Color textColor = { 255, 255, 255, 255 }; // Branco
            int yPos = 120;

            rendererPort->renderSimpleText("Desenvolvedor:", Config::windowSize.x / 2 - 200, yPos, textColor);
            yPos += 40;
            rendererPort->renderSimpleText("Santyero Mesquita Borges dos Santos", Config::windowSize.x / 2 - 180, yPos, textColor);
            yPos += 80;

            rendererPort->renderSimpleText("Professores:", Config::windowSize.x / 2 - 200, yPos, textColor);
            yPos += 40;
            rendererPort->renderSimpleText("Eduardo Henrique Molina da Cruz", Config::windowSize.x / 2 - 180, yPos, textColor);
            yPos += 40;
            rendererPort->renderSimpleText("Hélio Toshio Kamakawa", Config::windowSize.x / 2 - 180, yPos, textColor);
            yPos += 80;

            rendererPort->renderSimpleText("Templates:", Config::windowSize.x / 2 - 200, yPos, textColor);
            yPos += 40;
            rendererPort->renderSimpleText("tiny-swords", Config::windowSize.x / 2 - 180, yPos, textColor);

            // Renderizar instrução para voltar
            SDL_Color instructionColor = { 150, 150, 150, 255 }; // Cinza
            rendererPort->renderSimpleText("Press ESC to return to main menu", Config::windowSize.x / 2 - 150, Config::windowSize.y - 50, instructionColor);

            rendererPort->renderPresent();

            // Tratamento de eventos
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    backToMenu = true;
                }
                else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                    backToMenu = true;
                }
            }
        }
    }

    void GameEngine::showOptionsMenu() {
        bool exitOptions = false;
        SDL_Event event;

        while (!exitOptions) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    exitOptions = true;
                    currentState = GameState::Exit;
                }
                else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                    exitOptions = true;
                }
                else {
                    optionsMenu->handleInput(event);
                    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
                        if (optionsMenu->getSelectedOption() == 1) {
                            toggleFullscreen();
                        }
                        else if (optionsMenu->getSelectedOption() == 2) {
                            exitOptions = true;
                        }
                    }
                }
            }

            optionsMenu->render();
        }
    }

    void GameEngine::toggleFullscreen() {
        window.toggleFullscreen();
        optionsMenu->toggleFullscreen();
        // Reajuste o renderer se necessário
        SDL_RenderSetLogicalSize(static_cast<SDLRendererAdapter*>(rendererPort)->getRenderer(),
            window.getWidth(), window.getHeight());
    }


}
