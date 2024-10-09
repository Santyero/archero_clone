#pragma once
#ifndef GAME_H
#define GAME_H

#include "Window.h"
#include "Enemy.h"
#include "Player.h"
#include "Projectile.h"
#include "TimeServicePort.h"
#include "RendererPort.h"
#include "PhysicsEngine.h"
#include "Obstacle.h"
#include "TextureManager.h"
#include "MixerManager.h"  // Sistema de áudio
#include "HUD.h"
#include <list>
#include <memory>

namespace Game {

    class GameEngine
    {
    private:
        Window& window;
        RendererPort* rendererPort = nullptr;
        TimeServicePort* timeServicePort = nullptr;
        PhysicsEngine* physicsEngine = nullptr;
        std::list<Enemy> enemies;
        std::list<Obstacle> obstacles;
        std::list<Projectile> playerProjectiles;
        std::list<Projectile> enemyProjectiles;
        Player* player = nullptr;
        std::unique_ptr<TextureManager> textureManager;
        std::unique_ptr<MixerManager> mixerManager;
		int level = 1;
        int timerChangeLevel = 0;
        std::unique_ptr<HUD> hud;

        void loadAnimationFrames(std::vector<SDL_Rect>& targetFrames, int startY, int numFrames, int frameHeight, int frameWidth);
        void setupPlayerAnimations();
        void setupEnemyAnimations();
        void setupProjectiles();
        void setutpScene();
        void handlePauseMenuSelection(int selectedOption, SDL_bool& done);

    public:
        GameEngine(Window& window_, RendererPort* rendererPort_, TimeServicePort* timeServicePort_);
        ~GameEngine();

        void createWall();
        void createEnemies();
        void loadTextures();
        void loadAudio();  // Carregar arquivos de áudio
        void startGame();
        void loadElements();
        void updateCollisions();
        VisualElement* findNextElement(VisualElement* selectedElement, std::list<VisualElement*> elements);
        void spawnProjectiles(VisualElement* element, std::list<VisualElement*> elementsToFocus, std::list<Projectile>& projectileList, const std::string& projectileType);
		void changeLevel();
        void verifyRenderNewEnemies();

        MixerManager* getMixerManager() { return mixerManager.get(); }  // Acessar gerenciador de áudio

        template <typename T>
        std::list<VisualElement*> convertListToVisualElements(const std::list<T>& elements) {
            static_assert(std::is_base_of<VisualElement, T>::value, "T must be derived from VisualElement");
            std::list<VisualElement*> visualElements;
            for (const auto& element : elements) {
                visualElements.push_back(const_cast<VisualElement*>(static_cast<const VisualElement*>(&element)));
            }
            return visualElements;
        }
    };

}

#endif // GAME_H
