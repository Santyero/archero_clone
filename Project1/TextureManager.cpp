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

        auto loadFrames = [&](const std::vector<SDL_Rect>& frames, const std::string& animationName, bool flipHorizontally = false) {
            std::vector<SDL_Texture*> frameTextures;
            for (const auto& frame : frames) {
                SDL_Surface* frameSurface = SDL_CreateRGBSurface(0, frame.w, frame.h, 32, 0, 0, 0, 0);
                SDL_BlitSurface(surface, &frame, frameSurface, nullptr);

                if (flipHorizontally) {
                    SDL_Surface* flippedSurface = SDL_CreateRGBSurface(0, frame.w, frame.h, 32, 0, 0, 0, 0);
                    for (int y = 0; y < frame.h; ++y) {
                        for (int x = 0; x < frame.w; ++x) {
                            Uint32* pixels = (Uint32*)frameSurface->pixels;
                            Uint32* flippedPixels = (Uint32*)flippedSurface->pixels;
                            flippedPixels[y * frame.w + x] = pixels[y * frame.w + (frame.w - 1 - x)];
                        }
                    }
                    SDL_FreeSurface(frameSurface);
                    frameSurface = flippedSurface;
                }
                Uint32 colorkey = SDL_MapRGB(frameSurface->format, 0, 0, 0);
                SDL_SetColorKey(frameSurface, SDL_TRUE, colorkey);
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

        textures[id + "_idle"] = loadFrames(animInfo.idleFrames, "idle");
		textures[id + "_walk"] = loadFrames(animInfo.walkFrames, "walk");
		textures[id + "_shoot"] = loadFrames(animInfo.shootFrame, "shoot");
		textures[id + "_shootTop"] = loadFrames(animInfo.shootTopFrames, "shootTop");
		textures[id + "_shootAngularTop"] = loadFrames(animInfo.shootAngularTopFrames, "shootAngularTop");
		textures[id + "_shootAngularBottom"] = loadFrames(animInfo.shootAngularBottomFrames, "shootAngularBottom");
		textures[id + "_shootBottom"] = loadFrames(animInfo.shootBottomFrames, "shootBottom");

        SDL_FreeSurface(surface);
        animationInfos[id] = animInfo;
        return true;
    }

    SDL_Texture* TextureManager::getTexture(const std::string& id, const std::string& animationState, int frameIndex) const {
        std::string key = id + "_" + animationState;
        auto it = textures.find(key);
        /*if (id == "player") {
            std::cout << animationState << frameIndex << std::endl;
        }*/
        if (it != textures.end() && frameIndex < it->second.size()) {
            return it->second[frameIndex];
        }
        else if (it != textures.end()) {
            return it->second[0];
        }
        if (id == "player") {
			std::cout << "caiu null" << std::endl;
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