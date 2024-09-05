#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <unordered_map>
#include <string>
#include <vector>
#include <memory>

namespace Game {

    struct AnimationInfo {
        std::vector<SDL_Rect> idleFrames;
        std::vector<SDL_Rect> runningFrames;
        std::vector<SDL_Rect> shootingFrames;
    };

    class TextureManager {
    public:
        TextureManager(SDL_Renderer* renderer);
        ~TextureManager();

        bool loadTextures(const std::string& id, const std::string& filePath, const AnimationInfo& animInfo);
        SDL_Texture* getTexture(const std::string& id, const std::string& animationState, int frameIndex) const;
        size_t getFrameCount(const std::string& id, const std::string& animationState) const;
        void clearTextures();

    private:
        SDL_Renderer* renderer;
        std::unordered_map<std::string, std::vector<SDL_Texture*>> textures;
        std::unordered_map<std::string, AnimationInfo> animationInfos;
    };

}