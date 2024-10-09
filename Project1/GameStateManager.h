#pragma once
#include <memory>

namespace Game {
    class GameStateManager {
    private:
        static std::unique_ptr<GameStateManager> instance;
        bool isPaused;

        GameStateManager() : isPaused(false) {}

    public:
        static GameStateManager* getInstance() {
            if (!instance) {
                instance = std::unique_ptr<GameStateManager>(new GameStateManager());
            }
            return instance.get();
        }

        void setPaused(bool paused) {
            isPaused = paused;
        }

        bool getIsPaused() const {
            return isPaused;
        }

        // Adicione outros estados do jogo conforme necessário
    };
}