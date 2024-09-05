#include "TextureManager.h"
#include <iostream>

namespace Game {

    TextureManager::TextureManager(SDL_Renderer* renderer) : renderer(renderer) {}

    TextureManager::~TextureManager() {
        clearTextures();
    }

    bool TextureManager::loadTextures(const std::string& id, const std::string& filePath, const AnimationInfo& animInfo) {
        SDL_Surface* surface = IMG_Load(filePath.c_str());
        if (!surface) {
            std::cerr << "Failed to load texture file: " << filePath << std::endl;
            return false;
        }

        auto loadFrames = [&](const std::vector<SDL_Rect>& frames) {
            std::vector<SDL_Texture*> frameTextures;
            for (const auto& frame : frames) {
                SDL_Surface* frameSurface = SDL_CreateRGBSurface(0, frame.w, frame.h, 32, 0, 0, 0, 0);
                SDL_BlitSurface(surface, &frame, frameSurface, nullptr);
                SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, frameSurface);
                SDL_FreeSurface(frameSurface);
                if (!texture) {
                    std::cerr << "Failed to create texture from surface" << std::endl;
                    continue;
                }
                frameTextures.push_back(texture);
            }
            return frameTextures;
            };

        textures[id + "_idle"] = loadFrames(animInfo.idleFrames);
        textures[id + "_running"] = loadFrames(animInfo.runningFrames);
        textures[id + "_shooting"] = loadFrames(animInfo.shootingFrames);

        SDL_FreeSurface(surface);
        animationInfos[id] = animInfo;
        return true;
    }

    SDL_Texture* TextureManager::getTexture(const std::string& id, const std::string& animationState, int frameIndex) const {
        std::string key = id + "_" + animationState;

        bool isPlayer = (id == "player");
       //faca um if para comparar se é "player"
        /*if (id == "player") {
            std::cout << "player animation" << animationState << std::endl;
        }*/
        auto it = textures.find(key);
        if (it != textures.end() && frameIndex < it->second.size()) {
            return it->second[frameIndex];
        }
        return nullptr;
    }

    size_t TextureManager::getFrameCount(const std::string& id, const std::string& animationState) const {
        std::string key = id + "_" + animationState;
        auto it = textures.find(key);
        if (it != textures.end()) {
            return it->second.size();
        }
        return 0;
    }

    void TextureManager::clearTextures() {
        for (auto& pair : textures) {
            for (auto texture : pair.second) {
                SDL_DestroyTexture(texture);
            }
        }
        textures.clear();
        animationInfos.clear();
    }

}