#pragma once
#include "TimeServicePort.h"
#include <SDL.h>
#include <iostream>

namespace Game {
    class SDLTimeServiceAdapter : public TimeServicePort {
    private:
        int lastElapsedTimeInSeconds = 0;
        int lastCurrentTimeInSeconds = 0;
        int pauseStartTime = 0;
        int totalPausedTime = 0;
        bool isPaused = false;

    public:
        SDLTimeServiceAdapter() {
            // Inicialize a SDL se necessário
            if (SDL_Init(SDL_INIT_TIMER) != 0) {
                std::cerr << "Erro ao inicializar a SDL: " << SDL_GetError() << std::endl;
                // Lidar com o erro, lançar uma exceção, etc.
            }
        }

        virtual int getCurrentTimeInSeconds() override {
            return static_cast<int>(SDL_GetTicks() - totalPausedTime);
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

        virtual void pauseTime() override {
            if (!isPaused) {
                isPaused = true;
                pauseStartTime = SDL_GetTicks();
            }
        }

        virtual void resumeTime() override {
            if (isPaused) {
                isPaused = false;
                totalPausedTime += SDL_GetTicks() - pauseStartTime;
            }
        }

        virtual bool getIsPaused() override {
            return isPaused;
        }
    };
}