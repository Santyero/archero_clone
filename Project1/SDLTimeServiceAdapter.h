#pragma once
#include "TimeServicePort.h"
#include <SDL.h>
#include <iostream>

namespace Game {
    class SDLTimeServiceAdapter : public TimeServicePort {
    private:
        int lastElapsedTimeInSeconds = 0;
        int lastCurrentTimeInSeconds = 0;
    public:
        SDLTimeServiceAdapter() {
            // Inicialize a SDL se necessário
            if (SDL_Init(SDL_INIT_TIMER) != 0) {
                std::cerr << "Erro ao inicializar a SDL: " << SDL_GetError() << std::endl;
                // Lidar com o erro, lançar uma exceção, etc.
            }
        }

        virtual int getCurrentTimeInSeconds() override {
            return static_cast<int>(SDL_GetTicks());
        }

        virtual int getLastElapsedTimeInSeconds() override{
            return this->lastElapsedTimeInSeconds;
        }

        virtual void updateLastElapsedTimeInSeconds() override {
            this->lastElapsedTimeInSeconds = this->lastCurrentTimeInSeconds;
        }

        virtual int getLastCurrentTimeInSeconds() override {
            return this->lastCurrentTimeInSeconds;
        }

        virtual void updateLastCurrentTimeInSeconds() override {
            this->lastCurrentTimeInSeconds = this->getCurrentTimeInSeconds();
        }
    };
}