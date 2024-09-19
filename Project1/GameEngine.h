#pragma once
#ifndef GAME_H
#define GAME_H

#include "Window.h"
#include "Enemy.h"
#include "TimeServicePort.h"
#include "RendererPort.h"
#include "PhysicsEngine.h"
#include "Obstacle.h"
#include "TextureManager.h"
#include "MixerManager.h"  // Adicione esta linha
#include <list>
#include <memory>

class Player;

namespace Game {
    class Player;

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
        std::unique_ptr<MixerManager> mixerManager;  // Adicione esta linha

    public:
        GameEngine(Window& window_, RendererPort* rendererPort_, TimeServicePort* timeServicePort_);
        ~GameEngine();  // Adicione um destrutor

        void createWall();
        void createEnemies();
        void loadTextures();
        void loadAudio();  // Adicione este método
        void startGame();
        void loadElements();
        void updateCollisions();
        VisualElement* findNextElement(VisualElement* selectedElement, std::list<VisualElement*> elements);
        void spawnProjectiles(VisualElement* element, std::list<VisualElement*> elementsToFocus, std::list<Projectile>& projectileList);

        MixerManager* getMixerManager() { return mixerManager.get(); }  // Adicione este método

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