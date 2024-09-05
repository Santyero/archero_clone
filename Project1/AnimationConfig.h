#pragma once
#include <vector>
#include <string>
#include <SDL.h>

namespace Game {
    class ISprite {
    public:
        virtual ~ISprite() = default;
        virtual void update(float deltaTime) = 0;
    };

    class IAnimation : public ISprite {
    public:
        virtual void setFrames(const std::vector <SDL_Rect>& frameIndices) = 0;
        virtual SDL_Rect getCurrentFrame() = 0;
    };

    class ITextureLoader {
    public:
        virtual ~ITextureLoader() = default;
        virtual int loadTexture(const std::string& path) = 0;
        virtual void* getTextureData(int textureId) = 0;
    };

    class IRenderer {
    public:
        virtual ~IRenderer() = default;
        virtual void renderTexture(int textureId, SDL_Rect& rect, SDL_Rect& destRect) = 0;
    };

    class AnimationConfig {
    public:
        std::vector<SDL_Rect> idleFrames;
        std::vector<SDL_Rect> runningFrames;
        std::vector<SDL_Rect> shootingFrames;
    };

    class Animation : public IAnimation {
    public:
        Animation(float frameTime) : currentFrame(0), timer(0), frameTime(frameTime) {}

        void update(float deltaTime) override {
            timer += deltaTime;
            if (timer >= frameTime && !frames.empty()) {
                currentFrame = (currentFrame + 1) % static_cast<int>(frames.size());
                timer -= frameTime;
            }
        }

        void setFrames(const std::vector<SDL_Rect>& frameIndices) override {
            frames = frameIndices;
        }

        SDL_Rect getCurrentFrame() override {
            return frames[currentFrame];
        }

    private:
        std::vector <SDL_Rect> frames;
        int currentFrame;
        float timer;
        float frameTime;
    };

}